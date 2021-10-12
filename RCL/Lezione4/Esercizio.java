import java.util.concurrent.ThreadLocalRandom;

public class Esercizio
{
	static class Dropbox
	{
		private int buffer = -1;
		private boolean empty = true;
		public Dropbox() {}
		// toggled on when only even numbers are to be consumed
		public synchronized int take(boolean even) throws InterruptedException
		{
			while (empty || (buffer % 2 == 0) != even) this.wait();
			int res = buffer;
			empty = true;
			this.notifyAll();
			return res;
		}
		public synchronized void put(int value) throws InterruptedException
		{
			while (!empty) this.wait();
			buffer = value;
			empty = false;
			this.notifyAll();
			return;
		}
	}

	static class Producer implements Runnable
	{

		private Dropbox dropbox;
		private final int min = 0;
		private final int max = 100;

		public Producer(Dropbox dropbox) { this.dropbox = dropbox; }

		public void run()
		{
			while (true)
			{
				int value = ThreadLocalRandom.current().nextInt(min, max);
				try { dropbox.put(value); }
				catch (InterruptedException e) { return; }
				System.out.printf("[%d] Producer: %d has been saved.\n", Thread.currentThread().getId(), value);
				System.out.flush();
			}
		}

	}

	static class Consumer implements Runnable
	{

		private Dropbox dropbox;
		private boolean even;

		public Consumer(Dropbox dropbox, boolean even)
		{
			this.dropbox = dropbox;
			this.even = even;
		}

		public void run()
		{
			while (true)
			{
				int res;
				try { res = dropbox.take(even); }
				catch (InterruptedException e) { return; }
				System.out.printf("[%d] Consumer: %d has been consumed.\n", Thread.currentThread().getId(), res);
				System.out.flush();
			}
		}

	}

	public static void main(String[] args)
	{
		final long delay = 2000;
		Dropbox dropbox = new Dropbox();
		Thread c1 = new Thread(new Consumer(dropbox, true));
		Thread c2 = new Thread(new Consumer(dropbox, false));
		Thread p = new Thread(new Producer(dropbox));
		c1.start(); c2.start(); p.start();
		try
		{
			c1.join(delay); c2.join(delay); p.join(delay);
			if (c1.isAlive()) c1.interrupt();
			if (c2.isAlive()) c2.interrupt();
			if (p.isAlive()) p.interrupt();
		}
		catch (Exception e) { System.exit(1); }


	}
}