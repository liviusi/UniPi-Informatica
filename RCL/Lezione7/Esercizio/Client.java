package Esercizio;

import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

public class Client
{
	public static final int PORT_NO = 1234;
	public static final int RESPONSE_LEN = 1024;
	public static final String PONG_STRING = "PONG";
	public static final String PING_STRING = "PING";
	public static final int TIMEOUT = 1500;
	public static final String HOSTNAME = "localhost";
	public static void main(String[] args)
	{
		final byte[] PING_BYTE;
		try { PING_BYTE = PING_STRING.getBytes("US-ASCII"); }
		catch (UnsupportedEncodingException e) { return; }
		try (DatagramSocket dSocket = new DatagramSocket(0))
		{
			dSocket.setSoTimeout(TIMEOUT);
			InetAddress host = null;
			try { host = InetAddress.getByName(HOSTNAME); }
			catch (UnknownHostException e)
			{
				System.err.println("No IP address could be found for host localhost");
				System.exit(1);
			}
			final DatagramPacket request = new DatagramPacket(PING_BYTE, PING_BYTE.length, host , PORT_NO);
			final DatagramPacket response = new DatagramPacket(new byte[RESPONSE_LEN], RESPONSE_LEN);
			try { dSocket.send(request); }
			catch (Exception e) { return; }
			System.out.println("Sent: " + PING_STRING);
			while (true)
			{
				try { dSocket.receive(response); }
				catch (SocketTimeoutException e) { continue; }
				catch (Exception e) { return; }

				// ensure PONG has been received
				String received = null;
				try { received = new String(response.getData(), 0, response.getLength(), "US-ASCII"); }
				catch (UnsupportedEncodingException e) { return; }
				System.out.println("Received: " + received);
				if (received.equals(PONG_STRING)) break;
			}
		}
		catch (SocketException e) { return; }
	}
}