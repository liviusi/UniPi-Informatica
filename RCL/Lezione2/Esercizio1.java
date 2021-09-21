import java.util.concurrent.*;

public class Esercizio1
{

	class Visitatore implements Runnable
	{
		private int id = 0;
		public final int min = 0;
		public final int max = 1000;
		public Visitatore(int id)
		{
			this.id = id;
		}

		public void run()
		{
			System.out.println("Viaggiatore " + id + ": sto acquistando un biglietto.");
			try { Thread.sleep(ThreadLocalRandom.current().nextLong(min, max + 1)); } // 1000 incluso 
			catch (InterruptedException e) { }
			System.out.println("Viaggiatore " + id + ": ho acquistato il biglietto");
			return;
		}
	}

	public static void main(String[] args)
	{
		int viaggiatori = 50;
		int emettitrici = 5;
		int coda = 10;
		Esercizio1 es = new Esercizio1();

		BlockingQueue<Runnable> queue = new ArrayBlockingQueue<Runnable>(coda);
		ExecutorService pool = new ThreadPoolExecutor(emettitrici, emettitrici, 5000, TimeUnit.MILLISECONDS, queue);
		for (int i = 0; i < viaggiatori; i++)
		{
			try
			{
				pool.execute(es.new Visitatore(i));
				Thread.sleep(50);
			}
			catch (RejectedExecutionException | InterruptedException e) { System.out.println("Viaggiatore " + i + ": sala esaurita."); }
		}
		System.exit(0);
	}
}
