package Assignment;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class PingServer
{

	public static final int TIMEOUT = 1500;
	public static final int COREPOOLSIZE = 5;
	public static final int MAXIMUMPOOLSIZE = 20;
	public static final int KEEPALIVETIME = 500;

	public static final int REQUEST_LENGTH = 1024;
	public static final int BOUND = 100; // used when generating random values

	// returns whether stringPortNo is a legal port
	public static int assignPort(String stringPortNo) throws IllegalArgumentException
	{
		int portNo = 0;
		try
		{
			portNo = Integer.parseInt(stringPortNo);
			if (portNo <= 1024 || portNo > 65535) throw new IllegalArgumentException(); // portNo does not respect default boundaries
		}
		catch (NumberFormatException e) { throw new IllegalArgumentException(); }
		return portNo;
	}

	public static class ClientHandler implements Runnable
	{
		public static final int latencyFactor = 5; // factor input param "latency" is to be multiplied by
		private DatagramSocket dSocket = null;
		private DatagramPacket request = null;
		private boolean shouldBeLost = false; // toggled on when the server is not to respond
		private long latency = 0;


		public ClientHandler(DatagramSocket dSocket, DatagramPacket request, int shouldBeLost, int latency)
		{
			this.dSocket = dSocket;
			this.request = request;
			this.shouldBeLost = shouldBeLost < 25;
			this.latency = (long) (latency * latencyFactor);
		}

		public void run()
		{
			if (shouldBeLost) return;
			DatagramPacket response = new DatagramPacket(request.getData(), request.getLength(), request.getAddress(), request.getPort()); // echoes back the request
			try { Thread.sleep(latency); }
			catch (InterruptedException e) { } // ignore
			try { dSocket.send(response); }
			catch (IOException e) { } // ignore
		}
	}
	public static void main(String[] args)
	{
		int portNo = 0;
		long seed = 0;
		if (args.length != 0 && args.length <= 2)
		{
			try { portNo = assignPort(args[0]); }
			catch (IllegalArgumentException e)
			{
				System.err.println("ERR - arg 1");
				System.exit(1);
			}
			if (args.length == 2)
			{
				try { seed = Long.parseLong(args[1]); }
				catch (NumberFormatException e)
				{
					System.err.println("ERR - arg 2");
					System.exit(1);
				}
			}
		}
		else
		{
			System.err.println("Usage: java PingServer port [seed]");
			System.exit(1);
		}
		final ExecutorService pool = new ThreadPoolExecutor(COREPOOLSIZE, MAXIMUMPOOLSIZE, KEEPALIVETIME, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<>());
		Thread terminationHandler = new Thread()
		{
			public void run()
			{
				pool.shutdown();
				try { if (!pool.awaitTermination(TIMEOUT, TimeUnit.MILLISECONDS)) pool.shutdownNow(); }
				catch (Exception e) { pool.shutdownNow(); }
			}
		};
		Runtime.getRuntime().addShutdownHook(terminationHandler);
		final Random random = new Random(seed);
		DatagramSocket dSocket = null;
		try { dSocket =  new DatagramSocket(portNo); }
		catch (SocketException e)
		{
			System.err.println("ERR - arg 1");
			System.exit(1);
		}
		while (true)
		{
			DatagramPacket request = new DatagramPacket(new byte[REQUEST_LENGTH], REQUEST_LENGTH);
			try { dSocket.receive(request); }
			catch (IOException e) { }
			pool.submit(new Thread(new ClientHandler(dSocket, request, random.nextInt(BOUND), random.nextInt(BOUND))));
		}
	}
}
