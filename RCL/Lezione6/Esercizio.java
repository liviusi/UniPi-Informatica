import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.ServerSocket;
import java.net.Socket;

public class Esercizio
{
	final static String pathname = "./fileToSend.txt";
	final static File fileToSend = new File(pathname);
		public static void main(String[] args)
	{
		if (args.length != 1)
		{
			System.err.println("java Esercizio <port>");
			return;
		}
		final int port;
		try
		{
			port = Integer.parseInt(args[0]);
			// ensure range is valid
			if (port < 1024 || port > 65535) throw new Exception();
		}
		catch (Exception e)
		{
			System.err.println(args[0] + " is not a valid port number.");
			return;
		}
		try (ServerSocket ssocket = new ServerSocket(port))
		{
			while(true)
			{
				System.out.println("Waiting for new clients...");
				try (Socket s = ssocket.accept())
				{
					System.out.println("New client connected!");

					try
						(Writer w = new OutputStreamWriter(s.getOutputStream());
						BufferedReader b = new BufferedReader(new FileReader(fileToSend)))
					{
						String line;
						while ((line = b.readLine()) != null) w.write(line);
						w.write("\r\n");
						w.flush();
					}
					System.out.println("File content has been sent. Now closing connection...");
				}
			}
		}
		catch (Exception e) { e.printStackTrace(); return; }
	}
}
