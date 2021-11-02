package Esercizio;

import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.net.SocketTimeoutException;

public class Server
{
	public static final int PORT_NO = 1234;
	public static final int REQUEST_LEN = 1024;
	public static final String PONG_STRING = "PONG";
	public static final String PING_STRING = "PING";
	public static final int TIMEOUT = 1500;
	public static void main(String[] args)
	{
		final byte[] PONG_BYTE;
		try { PONG_BYTE = PONG_STRING.getBytes("US-ASCII"); }
		catch (UnsupportedEncodingException e) { return; }
		int maxRequestNo = 1;
		if (args.length == 1)
		{
			try
			{
				maxRequestNo = Integer.parseInt(args[0]);
				if (maxRequestNo <= 0) throw new IllegalArgumentException();
			}
			catch (Exception e)
			{
				System.err.printf("%s is not a valid integer. Run as Server [maxRequestNo].", args[0]);
				System.exit(1);
			}
		}
		int i = 0;
		while (i != maxRequestNo)
		{
			try (final DatagramSocket dSocket = new DatagramSocket(PORT_NO))
			{
				DatagramPacket request = new DatagramPacket(new byte[REQUEST_LEN], REQUEST_LEN);
				try { dSocket.receive(request); }
				catch (SocketTimeoutException e) { continue; }
				catch (Exception e)
				{
					e.printStackTrace();
					break;
				}
				String received = null;
				try { received = new String(request.getData(), 0, request.getLength(), "US-ASCII"); }
				catch (UnsupportedEncodingException e) { return; }
				System.out.println("Received: " + received);
				if (!received.equals(PING_STRING)) continue; // ignore every input different from "PING"
				DatagramPacket response = new DatagramPacket(PONG_BYTE, PONG_BYTE.length, request.getAddress(), request.getPort());
				try { dSocket.send(response); }
				catch (Exception e)
				{
					e.printStackTrace();
					break;
				}
				System.out.println("Sent: " + PONG_STRING);
				i++;
			}
			catch (SocketException e) { return; }
		}
	}
}