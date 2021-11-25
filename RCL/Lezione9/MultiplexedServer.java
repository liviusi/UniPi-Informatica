import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.nio.BufferOverflowException;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.ClosedSelectorException;
import java.nio.channels.SelectableChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.StandardCharsets;
import java.util.Iterator;

public class MultiplexedServer
{
	public static final int PORT_NO = 6789;
	public static final int BUFFERSIZE = 2048;
	public static final int TIMEOUT = 1500;

	public static final int COREPOOLSIZE = 5;
	public static final int MAXIMUMPOOLSIZE = 20;
	public static final int KEEPALIVETIME = 500;

	public static final String ECHOED = " echoed by server";
	public static final String QUIT_STRING = ":q!";
	public static Selector selector = null;


	public static void removeClient(SelectionKey k, SocketChannel client) throws IOException
	{
		k.cancel();
		client.close();
	}

	public static void registerAccept(SelectionKey k) throws IOException, ClosedChannelException
	{
		ServerSocketChannel server = (ServerSocketChannel) k.channel();
		SocketChannel client = null;
		client = server.accept();
		client.configureBlocking(false);

		client.register(selector, SelectionKey.OP_READ, ByteBuffer.allocate(BUFFERSIZE));
		System.out.println("New client accepted: " + client.getLocalAddress());
	}

	public static void handleRead(SelectionKey key)
	{
		ByteBuffer buffer = (ByteBuffer) key.attachment();
		SocketChannel client = (SocketChannel) key.channel();
		int nRead = 0;
		buffer.flip();
		buffer.clear();
		try { nRead = client.read(buffer); }
		catch (ClosedChannelException e) { return; }
		catch (IOException e)
		{
			System.err.printf("I/O error occurred:\n%s\n", e.getMessage());
			try { removeClient(key, client); }
			catch (IOException ignored) { }
			return;
		}
		if (nRead == -1) return;
		else if (nRead == 0) return;
		else
		{
			buffer.flip();
			String message = StandardCharsets.UTF_8.decode(buffer).toString();
			try { System.out.printf("> client %s: \"%s\"\n", client.getLocalAddress(), message); }
			catch (IOException e)
			{
				System.err.printf("I/O error occurred:\n%s\nNow removing client.\n", e.getMessage());
				try { removeClient(key, client); }
				catch (IOException ignored) { }
				return;
			}
			if (!message.equals(QUIT_STRING))
			{
				client.keyFor(selector).interestOps(SelectionKey.OP_WRITE);
				buffer.rewind();
				buffer.compact();
				client.keyFor(selector).attach(buffer);
			}
			else
			{
				System.out.println("QUIT string received! Now removing client...");
				try { removeClient(key, client); }
				catch (IOException ignored) { }
			}
			return;
		}	
	}

	public static void handleWrite(SelectionKey key)
	{
		ByteBuffer buffer = (ByteBuffer) key.attachment();
		SocketChannel client = (SocketChannel) key.channel();
		try { client.configureBlocking(false); }
		catch (ClosedChannelException e)
		{
			try { removeClient(key, client); }
			catch (IOException ignored) { }
			return;
		}
		catch (IOException e)
		{
			System.err.printf("I/O error occurred:\n%s\nNow removing client.\n", e.getMessage());
			try { removeClient(key, client); }
			catch (IOException ignored) { }
			return;
		}
		System.out.printf("position = %4d, limit = %4d, capacity = %4d%n", buffer.position(), buffer.limit(), buffer.capacity());
		try { buffer.put(ECHOED.getBytes(StandardCharsets.UTF_8)); }
		catch (BufferOverflowException e) { System.err.println("Cannot add \"echo\" to string."); }
		buffer.flip();
		try { client.write(buffer); }
		catch (ClosedChannelException e) { return; }
		catch (IOException e)
		{
			System.err.printf("I/O error occurred:\n%s\nNow removing client.\n", e.getMessage());
			try { removeClient(key, client); }
			catch (IOException ignored) { }
		}
		buffer.clear();

		client.keyFor(selector).interestOps(SelectionKey.OP_READ);
		client.keyFor(selector).attach(buffer);
	}

	public static void main(String[] args)
	{
		ServerSocketChannel serverSocketChannel = null;
		try
		{
			serverSocketChannel = ServerSocketChannel.open();
			ServerSocket sSocket = serverSocketChannel.socket();
			sSocket.bind(new InetSocketAddress(PORT_NO));
			serverSocketChannel.configureBlocking(false);
			selector = Selector.open();
			serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
		}
		catch (IOException e)
		{
			System.err.printf("I/O error occurred:\n%s\n", e.getMessage());
			System.exit(1);
		}
		System.out.printf("Server is now running on port %d.\nClients can type \"%s\" to quit connection.\n", PORT_NO, QUIT_STRING);
		final Selector selectorHandler = selector;
		Thread terminationHandler = new Thread()
		{
			public void run()
			{
				System.out.printf("\nServer has now entered shutdown mode.\n");
				selectorHandler.wakeup();
				Iterator<SelectionKey> keys = selectorHandler.keys().iterator();
				while (keys.hasNext())
				{
					SelectableChannel c = keys.next().channel();
					if (c instanceof ServerSocketChannel)
					{
						try { c.close(); }
						catch (IOException e) { System.err.printf("I/O error occurred during shutdown:\n%s\n.", e.getMessage()); }
					}
				}
				try { selectorHandler.close(); }
				catch (IOException e) { System.err.printf("I/O error occurred during shutdown:\n%s\n.", e.getMessage()); }
			}
		};
		Runtime.getRuntime().addShutdownHook(terminationHandler);
		while (true)
		{
			int r = 0;
			try { r = selector.select(); }
			catch (IOException e)
			{
				System.err.printf("I/O error occurred during select:\n%s\n", e.getMessage());
				System.exit(1);
			}
			catch (ClosedSelectorException e) { break; }
			if (!selector.isOpen()) break;
			if (r == 0) continue;
			Iterator<SelectionKey> keys = selector.selectedKeys().iterator();
			while (keys.hasNext())
			{
				SelectionKey k = keys.next();
				keys.remove(); // remove from selected set
				if (k.isAcceptable())
				{
					try { registerAccept(k); }
					catch (ClosedChannelException e) { System.err.printf("Error occurred during client registration:\n%s\n", e.getMessage()); }
					catch (IOException e)
					{
						System.err.printf("I/O error occurred:\n%s\n", e.getMessage());
						System.exit(1);
					}
					continue;
				}
				if (k.isReadable())
				{
					handleRead(k);
					continue;
				}
				if (k.isWritable())
				{
					handleWrite(k);
					continue;
				}
			}
		}
	}
}