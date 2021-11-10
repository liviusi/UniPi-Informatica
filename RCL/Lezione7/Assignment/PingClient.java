package Assignment;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

public class PingClient
{
	final static int SEQ_NO = 10;
	final static int TIMEOUT = 500;
	final static int RESPONSE_LENGTH = 1024;
	public static void main(String[] args)
	{
		if (args.length != 2)
		{
			System.err.println("Usage: java PingClient hostname port");
			System.exit(1);
		}
		final String hostname = args[0];
		int portNo = 0, lostPacketsNo = 0;
		long diffTime = 0;
		long minTime = 0, maxTime = 0, sum = 0;
		InetAddress host = null;
		try
		{
			portNo = Integer.parseInt(args[1]);
			if (portNo <= 1024 || portNo > 65535) throw new Exception();
		}
		catch (Exception e)
		{
			System.err.println("ERR - arg 2");
			System.exit(1);
		}
		try { host = InetAddress.getByName(hostname); }
		catch (UnknownHostException e)
		{
			System.err.printf("ERR - arg 1");
			System.exit(1);
		}
		for (int i = 0; i < SEQ_NO; i++)
		{
			try (final DatagramSocket dSocket = new DatagramSocket(0))
			{
				dSocket.setSoTimeout(TIMEOUT);
				final long sentAtTime = System.currentTimeMillis();
				final String message = "PING " + i + sentAtTime;
				final byte[] requestBuffer = message.getBytes();
				final DatagramPacket request = new DatagramPacket(requestBuffer, requestBuffer.length, host, portNo);
				try { dSocket.send(request); }
				catch (IOException e)
				{
					System.err.println("Fatal error occurred while sending " + message + ".");
					System.exit(1);
				}
				final DatagramPacket response = new DatagramPacket(new byte[RESPONSE_LENGTH], RESPONSE_LENGTH);
				while (true)
				{
					try { dSocket.receive(response); }
					catch (SocketTimeoutException t) // response has not been sent on time
					{
						System.out.println(message + " RTT: *");
						lostPacketsNo++;
						break;
					}
					catch (IOException e)
					{
						System.err.println("Fatal error occurred while attempting to receive " + message + ".");
						System.exit(1);
					}
					if (message.equals(new String(response.getData(), 0, response.getLength()))) // check whether the very same message has been received
					{
						diffTime = System.currentTimeMillis() - sentAtTime;
						if (i != 0)
						{
							minTime = Math.min(diffTime, minTime);
							maxTime = Math.max(diffTime, maxTime);
						}
						else
						{
							minTime = diffTime;
							maxTime = diffTime;
						}
						sum += diffTime;
						System.out.println(message + " RTT: " + diffTime + " ms");
						break;
					}
				}
			}
			catch (SocketException e)
			{
				e.printStackTrace();
				System.exit(1);
			}
		}
		System.out.println("---- PING Statistics ----");
		System.out.println(SEQ_NO + " packets transmitted, " + (SEQ_NO - lostPacketsNo) + " packets received, " + (lostPacketsNo * 100 / SEQ_NO) + "% packet loss");
		System.out.printf("round-trip (ms) min/avg/max = %d/%.2f/%d", minTime, (sum / (float) SEQ_NO), maxTime);
	}
}
