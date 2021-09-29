import java.util.concurrent.*;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class Esercizio
{

	static final int numThreads = 20; // number of threads allowed
	static final int timeout = 5000;

	abstract class Counter
	{
		protected int counter;

		abstract int get();

		abstract void increment();
	}

	class Writer implements Runnable
	{
		private Counter counter; // pointer to Counter instance

		public Writer(Counter counter) { this.counter = counter; }

		public void run() { counter.increment(); }
	}

	class Reader implements Runnable
	{
		private Counter counter; // pointer to Counter instance

		public Reader(Counter counter)  { this.counter = counter; }

		public void run() { System.out.println("Reader " + Thread.currentThread().getId() + " got " + counter.get()); }
	}

	class RLCounter extends Counter
	{
		private Lock lock;

		public RLCounter()
		{
			counter = 0;
			lock = new ReentrantLock();
		}

		public int get()
		{
			int currentCounterValue;
			try
			{
				lock.lock();
				currentCounterValue = counter;
			}
			finally { lock.unlock(); }
			return currentCounterValue;
		}

		public void increment()
		{
			try
			{
				lock.lock();
				counter++;
			}
			finally { lock.unlock(); }
		}
	}

	class RWCounter extends Counter
	{
		private ReadWriteLock lock;

		public RWCounter()
		{
			counter = 0;
			lock = new ReentrantReadWriteLock();
		}

		public int get()
		{
			int currentCounterValue;
			try
			{
				lock.readLock().lock();
				currentCounterValue = counter;
			}
			finally { lock.readLock().unlock(); }
			return currentCounterValue;
		}

		public void increment()
		{
			try
			{
				lock.writeLock().lock();
				counter++;
			}
			finally { lock.writeLock().unlock(); }
		}
	}

	public static long runTests(ExecutorService threadPool, Counter counter)
	{
		Esercizio esercizio = new Esercizio();
		long start = System.currentTimeMillis();
		// submit tasks
		for (int i = 0; i < numThreads; i++) threadPool.execute(esercizio.new Writer(counter));
		for (int i = 0; i < numThreads; i++) threadPool.execute(esercizio.new Reader(counter));
		// terminate
		threadPool.shutdown();
		try
		{
			if (!threadPool.awaitTermination(timeout, TimeUnit.MILLISECONDS))
				threadPool.shutdownNow();
		}
		catch (Exception e) { threadPool.shutdownNow(); }
		return System.currentTimeMillis() - start;
	}

	public static void main(String[] args)
	{
		Esercizio esercizio = new Esercizio();
		
		Counter counter1 = esercizio.new RLCounter();
		ExecutorService threadPool1 = Executors.newCachedThreadPool();
		System.out.println("\tRunning test with reentrant lock: " + runTests(threadPool1, counter1) + "ms.");

		Counter counter2 = esercizio.new RWCounter();
		ExecutorService threadPool2 = Executors.newCachedThreadPool();
		System.out.println("\tRunning test with rw lock: " + runTests(threadPool2, counter2) + "ms.");
	}
}
