import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.nio.BufferOverflowException;
import java.nio.ByteBuffer;
import java.nio.channels.CancelledKeyException;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.ClosedSelectorException;
import java.nio.channels.SelectableChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.StandardCharsets;
import java.util.Iterator;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class MultithreadedServer
{

	static interface RequestHandler
	{
		void handleRequest(final SelectionKey key, ByteBuffer buffer, final SocketChannel client);
	}

	public static class ClientHandler implements Runnable
	{
		public static enum ClientType implements RequestHandler
		{
			READER
			{
				public void handleRequest(final SelectionKey key, ByteBuffer buffer, final SocketChannel client)
				{
					int nRead = 0;
					buffer.flip();
					buffer.clear();
					try { nRead = client.read(buffer); }
					catch (ClosedChannelException e) { return; }
					catch (IOException e)
					{
						System.err.printf("I/O error occurred:\n%s\n", e.getMessage());
						try { client.close(); }
						catch (IOException ignored) { }
						return;
					}
					if (nRead == -1) return;
					else if (nRead == 0) return;
					else
					{
						buffer.flip();
						final String message = StandardCharsets.UTF_8.decode(buffer).toString();
						final String clientName;
						try { clientName = client.getLocalAddress().toString(); }
						catch (IOException e)
						{
							System.err.printf("I/O error occurred:\n%s\nNow removing client.\n", e.getMessage());
							try { client.close(); }
							catch (IOException ignored) { }
							return;
						}
						System.out.printf("> client %s: \"%s\"\n", clientName, message);
						if (!message.equals(QUIT_STRING))
						{
							buffer.rewind();
							buffer.compact();
							toBeRegistered.add(new SetElement(client, SelectionKey.OP_WRITE, buffer));
							selector.wakeup();
							return;
						}
						else
						{
							try { client.close(); }
							catch (IOException ignored) { }
							return;
						}
					}	
				}
			},
			WRITER
			{
				public void handleRequest(final SelectionKey key, ByteBuffer buffer, final SocketChannel client)
				{
					try { client.configureBlocking(false); }
					catch (ClosedChannelException e)
					{
						try { client.close(); }
						catch (IOException ignored) { }
						return;
					}
					catch (IOException e)
					{
						try { client.close(); }
						catch (IOException ignored) { }
						return;
					}
					try { buffer.put(ECHOED.getBytes(StandardCharsets.UTF_8)); }
					catch (BufferOverflowException e) { System.err.println("Cannot add \"echo\" to string."); }
					buffer.flip();
					try { client.write(buffer); }
					catch (ClosedChannelException e) { return; }
					catch (IOException e)
					{
						System.err.printf("I/O error occurred:\n%s\nNow removing client.\n", e.getMessage());
						try { client.close(); }
						catch (IOException ignored) { }
					}
					buffer.clear();
					toBeRegistered.add(new SetElement(client, SelectionKey.OP_READ, buffer));
					selector.wakeup();
					return;
				}
			}
		};

		private SocketChannel client = null;
		private ByteBuffer buffer = null;
		private SelectionKey key = null;
		private ClientType clientType = null;

		public ClientHandler(final SelectionKey key, final ClientType clientType)
		{
			this.key = key;
			this.client = (SocketChannel) key.channel();
			this.buffer = (ByteBuffer) key.attachment();
			this.clientType = clientType;
		}

		@Override
		public void run()
		{
			clientType.handleRequest(key, buffer, client);
			return;
		}

		public static void executeClientHandler(final SelectionKey key, final ClientHandler.ClientType clientType, final ExecutorService threadPool)
		{
			Thread t = new Thread(new ClientHandler(key, clientType));
			threadPool.execute(t);
		}
	}

	public static class SetElement
	{
		final SocketChannel client;
		final int operation;
		final ByteBuffer buffer;

		public SetElement(final SocketChannel client, final int operation, ByteBuffer buffer) throws IllegalArgumentException
		{
			if (operation != SelectionKey.OP_READ && operation != SelectionKey.OP_WRITE)
				throw new IllegalArgumentException("Operation specified is not valid. Only OP_READ and OP_WRITE are permitted.");
			this.client = client;
			this.operation = operation;
			this.buffer = buffer;
		}
	}

	public static final int PORT_NO = 6789;
	public static final int BUFFERSIZE = 2048;

	public static final int THREADPOOL_TIMEOUT = 1500;
	public static final int COREPOOLSIZE = 5;
	public static final int MAXIMUMPOOLSIZE = 20;
	public static final int KEEPALIVETIME = 500;

	public static final String ECHOED = " echoed by server";
	public static final String QUIT_STRING = ":q!";
	public static Selector selector = null;
	public static Set<SetElement> toBeRegistered = null; // set of elements to be (re-)registered by main thread


	public static void registerAccept(final SelectionKey k) throws IOException, ClosedChannelException
	{
		final ServerSocketChannel server = (ServerSocketChannel) k.channel();
		SocketChannel client = null;
		client = server.accept();
		client.configureBlocking(false);

		client.register(selector, SelectionKey.OP_READ, ByteBuffer.allocate(BUFFERSIZE));
		System.out.println("New client accepted: " + client.getLocalAddress());
	}


	public static void main(String[] args)
	{
		ServerSocketChannel serverSocketChannel = null;
		final ExecutorService threadPool = new ThreadPoolExecutor(COREPOOLSIZE, MAXIMUMPOOLSIZE, KEEPALIVETIME, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<>());
		try
		{
			serverSocketChannel = ServerSocketChannel.open();
			final ServerSocket sSocket = serverSocketChannel.socket();
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
		System.out.printf("Server is now running on port %d.\nClients can type \"%s\" to quit connection.\nServer can be turned off by hitting \"CTRL-C.\"\n", PORT_NO, QUIT_STRING);
		toBeRegistered = ConcurrentHashMap.newKeySet();
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
				threadPool.shutdown();
				try
				{
					if (!threadPool.awaitTermination(THREADPOOL_TIMEOUT, TimeUnit.MILLISECONDS))
						threadPool.shutdownNow();
				}
				catch (InterruptedException e) { threadPool.shutdownNow(); }
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
			if (r == 0)
			{
				if (!toBeRegistered.isEmpty())
				{
					for (final SetElement s : toBeRegistered)
					{
						try { s.client.register(selector, s.operation, s.buffer); }
						catch (ClosedChannelException clientDisconnected) { }
						toBeRegistered.remove(s);
					}
				}
			}
			final Iterator<SelectionKey> keys = selector.selectedKeys().iterator();
			while (keys.hasNext())
			{
				try
				{
					final SelectionKey k = keys.next();
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
						k.cancel();
						ClientHandler.executeClientHandler(k, ClientHandler.ClientType.READER, threadPool);
						continue;
					}
					if (k.isWritable())
					{
						k.cancel();
						ClientHandler.executeClientHandler(k, ClientHandler.ClientType.WRITER, threadPool);
						continue;
					}
				}
				catch (CancelledKeyException e) { continue; }
			}
		}
	}
}