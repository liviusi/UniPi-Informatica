import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;
import java.nio.charset.StandardCharsets;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class TimeServer
{

	public static final int SLEEP_TIME = 500;
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

		final MulticastSocket socketHandler = socket;
		Runtime.getRuntime().addShutdownHook(new Thread(new Runnable()
		{
			@Override
			public void run()
			{
				System.out.println("\nServer is now terminating: now freeing resources.\n");
				socketHandler.close();
			}
		}));

		System.out.printf("Server is now running on %s:%s.\n", args[0], args[1]);

		while (true)
		{
			byte[] time = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss").format(LocalDateTime.now()).getBytes(StandardCharsets.UTF_8);
			DatagramPacket message = new DatagramPacket(time, time.length, dategroup, portNumber);
			try { socket.send(message); }
			catch (IOException e) { throw new RuntimeException(e); }
			try { Thread.sleep(SLEEP_TIME); }
			catch (InterruptedException ignored) {  }
		}
	}
}