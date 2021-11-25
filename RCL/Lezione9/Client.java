import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class Client
{
	public static final String HOSTNAME = "localhost";
	public static final int PORT_NO = 6789;
	public static final String QUIT_STRING = ":q!";
	public static final int BUFFER_SIZE = 512;
	public static void main(String[] args)
	{
		SocketChannel client = null;
		try
		{
			client = SocketChannel.open(new InetSocketAddress(HOSTNAME, PORT_NO));
			System.out.println("Connected to server successfully!");
		}
		catch (IOException e)
		{
			System.err.printf("I/O error occurred:\n%s\n", e.getMessage());
			System.exit(1);
		}
		Scanner scanner = new Scanner(System.in);
		System.out.printf("Messages will be sent to the server after hitting \"ENTER\".\nYou can type \":q!\" to quit.\n");
		loop:
			while (true)
			{
				String s = scanner.nextLine();
				ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
				buffer.put(s.getBytes(StandardCharsets.UTF_8));
				buffer.flip();
				while (buffer.hasRemaining())
				{
					try { client.write(buffer); }
					catch (IOException e)
					{
						System.out.printf("I/O error occurred:\n%s\nNow disconnecting...", e.getMessage());
						break loop;
					}
				}
				StringBuilder response = new StringBuilder();
				buffer.clear();
				try { client.read(buffer); }
				catch (IOException e)
				{
					System.out.printf("I/O error occurred:\n%s\nNow disconnecting...", e.getMessage());
					break;
				}
				buffer.flip();
				while (buffer.hasRemaining()) response.append((char) buffer.get());
				String r = response.toString();
				if (!s.contains(QUIT_STRING))
				{
					if (!r.isEmpty())
						System.out.printf("Received from server: \"%s\"\n", r);
					else
					{
						System.out.println("Server has been turned off. No response has been received.");
						break;
					}
				}
				else break;
			}
		System.out.println("Client is now freeing resources...");
		try { client.close(); }
		catch (IOException ignored) { }
		scanner.close();
	}
}
