import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public class TimeClient
{

	public static final int PACKETS_TO_BE_RECEIVED_NO = 10;
	public static final int BUFFERLEN = 128;
	public static final int TIMEOUT = 500;
	public static void main(String[] args)
	{
		InetAddress dategroup = null;
		MulticastSocket socket = null;
		int portNumber = 0;

		if (args.length != 2)
		{
			System.err.println("Usage: java TimeServer <multicast IP address> <port number>");
			System.exit(1);
		}

		try
		{
			dategroup = InetAddress.getByName(args[0]);
			portNumber = Integer.parseInt(args[1]);
			if (portNumber < 1024 || portNumber > 65535) throw new IllegalArgumentException("Specified port number is not in the valid range.");
		}
		catch (UnknownHostException e)
		{
			System.err.printf("Host with name %s could not be found:\n%s\nNow terminating...\n", args[0], e.getMessage());
			System.exit(1);
		}
		catch (NumberFormatException e)
		{
			System.err.printf("String %s does not contain a valid integer:\n%s\n", args[1], e.getMessage());
			System.exit(1);
		}
		catch (IllegalArgumentException e)
		{
			System.err.printf("%s is not a valid port number:\n%s\n", args[1], e.getMessage());
			System.exit(1);
		}
		try { socket = new MulticastSocket(portNumber); }
		catch (IOException e)
		{
			System.err.printf("Multicast socket could not be istantiated on port %d:\n%s\nNow terminating...\n", portNumber, e.getMessage());
			System.exit(1);
		}
		try { socket.joinGroup(dategroup); }
		catch (IOException e)
		{
			System.err.printf("Socket could not join multicast dategroup:\n%s\nNow terminating...\n", e.getMessage());
			System.exit(1);
		}
		try { socket.setSoTimeout(TIMEOUT); }
		catch (SocketException e)
		{
			System.err.printf("Could not set timeout for socket:\n%s\n", e.getMessage());
			System.exit(1);
		}
		System.out.printf("Client has been subscribed to %s:%s successfully.\nNow waiting for messages to be received...\n", args[0], args[1]);
		for (int i = 0; i < PACKETS_TO_BE_RECEIVED_NO; i++)
		{
			byte[] tmp = new byte[BUFFERLEN];
			DatagramPacket packet = new DatagramPacket(tmp, tmp.length);
			try { socket.receive(packet); }
			catch (SocketTimeoutException e)
			{
				System.err.printf("Timeout! Server has either been turned off or has yet to be turned on:\n%s\n", e.getMessage());
				break;
			}
			catch (IOException e)
			{
				System.err.printf("Server has been turned off.\nNow terminating...\n");
				break;
			}
			System.out.printf("%d : %s\n", i + 1, StandardCharsets.UTF_8.decode(ByteBuffer.wrap(packet.getData())).toString());
		}
		try { socket.leaveGroup(dategroup); }
		catch (IOException ignored) {  }
		socket.close();
	}
}
