import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.ServerSocket;
import java.net.Socket;

/**
scrivere un programma JAVA che implementi un server Http che gestisca richieste
di trasferimento di file di diverso tipo (es. immagini jpeg, gif) provenienti da un
browser web.
●
il server
● sta in ascolto su una porta nota al client (es. 6789)
● gestisce richieste Http di tipo GET alla Request URL
localhost:port/filename
● le connessioni possono essere non persistenti.
● usare le classi Socket e ServerSocket per sviluppare il programma server
● per inviare al server le richieste, utilizzare un qualsiasi browser
 */

public class Assignment
{
	static final int PORT_NUMBER = 6789; // port to listen on
	static final String GET_STRING = "GET";
	static final String CONTENTLENGHT_STRING = "Content-Length: ";
	static final String CONTENTTYPE_STRING = "Content-Type: ";
	static final String CONNECTIONCLOSE_STRING = "Connection: close";

	// handle some content types
	public static String getContentType(final String filename)
	{
		switch (filename.substring(filename.lastIndexOf(".") + 1))
		{
			case "jpg":
			case "jpeg":
				return "image/jpeg";
			
			case "png":
				return "image/png";
			
			case "gif":
				return "image/gif";

			case "json":
				return "application/json";
			
			case "pdf":
				return "application/pdf";
			
			case "txt":
				return "text/plain";
		}
		return "application/octet-stream";
	}
	public static void main(String[] args)
	{
		final int requestNo; // number of requests to be handled
		if (args.length != 1) requestNo = 1;
		else
		{
			try
			{
				requestNo = Integer.parseInt(args[0]);
				if (requestNo <= 0) throw new IllegalArgumentException("Number of requests must be greater than zero.");
			}
			catch (Exception e)
			{
				e.printStackTrace();
				return;
			}
		}
		int requestsHandled = 0;
		try (final ServerSocket ssocket = new ServerSocket(PORT_NUMBER))
		{
			while (requestsHandled < requestNo)
			{
				try
					(final Socket s = ssocket.accept(); // connections are not persistent
					final BufferedReader socketInputStream = new BufferedReader(new InputStreamReader(s.getInputStream())))
				{
					final String request = socketInputStream.readLine();
					// +1 for the space between "GET" and the resource the client is asking for
					final String tmp = request.substring(GET_STRING.length() + 1, request.length());
					final String httpVersion = tmp.substring(tmp.indexOf(" ") + 1, tmp.length());
					System.out.println("NEW REQUEST: " + request);
					if (request.startsWith(GET_STRING))
					{
						final String fileName = java.net.URLDecoder.decode(tmp.substring(0, tmp.indexOf(" ")), "UTF-8");
						// locate fileName, read its contents and send it
						final File f = new File(fileName);
						System.out.println(fileName + " has been requested.");
						if (!f.exists()) // fileName does not exist on this server
						{
							try (Writer w = new OutputStreamWriter(s.getOutputStream()))
							{
								final String error = fileName + " does not identify a file on this server.";
								w.write(httpVersion + " 404 Not found.\r\n");
								w.write(CONTENTLENGHT_STRING + error.length() + "\r\n");
								w.write(CONTENTTYPE_STRING + "text/plain\r\n");
								w.write(CONNECTIONCLOSE_STRING + "\r\n\r\n");
								w.write(error + "\r\n");
								w.flush();
								System.out.println("\t" + error);
							}
						}
						else
						{
							if (!f.isFile()) // fileName is not a regular file
							{
								try (Writer w = new OutputStreamWriter(s.getOutputStream()))
								{
									final String error = fileName + " does not identify a regular file on this server.";
									w.write(httpVersion + " 422 Unprocessable entity.\r\n");
									w.write(CONTENTLENGHT_STRING + error.length() + "\r\n");
									w.write(CONTENTTYPE_STRING + "text/plain\r\n");
									w.write(CONNECTIONCLOSE_STRING + "\r\n\r\n");
									w.write(error + "\r\n");
									w.flush();
									System.out.println("\t" + error);
								}
							}
							else
							{
								// send file, close connection
								try
									(final BufferedInputStream fileReader = new BufferedInputStream(new FileInputStream(f));
									final BufferedOutputStream w = new BufferedOutputStream((s.getOutputStream())))
								{
									final byte[] contents = fileReader.readAllBytes();
									w.write((httpVersion + " 200 OK\r\n").getBytes());
									w.write((CONTENTLENGHT_STRING + contents.length + "\r\n").getBytes());
									w.write((CONTENTTYPE_STRING + getContentType(fileName) + "\r\n").getBytes());
									w.write((CONNECTIONCLOSE_STRING + "\r\n\r\n").getBytes());
									w.write(contents);
									w.write(("\r\n").getBytes());
									w.flush();
								}
								System.out.println("\tFile " + fileName + " has been sent.");
							}
						}
					}
					else
					{
						// only GET requests are to be handled
						try (Writer w = new OutputStreamWriter(s.getOutputStream()))
						{
							final String error = "Only HTTP GET requests are handled.";
							w.write(httpVersion + " 501 Not implemented.\r\n");
							w.write(CONTENTLENGHT_STRING + error.length() + "\r\n");
							w.write(CONTENTTYPE_STRING + "text/plain\r\n");
							w.write(CONNECTIONCLOSE_STRING + "\r\n\r\n");
							w.write(error + "\r\n");
							w.flush();
							System.out.println("\t" + error);
						}
					}
				}
				requestsHandled++;
			}
		}
		catch (IOException e)
		{
			e.printStackTrace();
			return;
		}
	}
}