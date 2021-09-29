import java.util.Random;
import java.util.concurrent.*;

public class Facoltativo
{

	class Persona implements Runnable
	{
		private int id;
		final static long origin = 0;
		final static long bound = 1000;

		public Persona(int id) { this.id = id; }

		public int getId() { return this.id; }

		public void run()
		{
			// passa dalla sala d'attesa alla seconda sala
			System.out.println("Visitatore " + id + ": uscito dalla prima sala.");
			System.out.println("Visitatore " + id + ": entrato nella seconda sala.");
			// svolge l'operazione allo sportello
			try { Thread.sleep(ThreadLocalRandom.current().nextLong(origin, bound + 1)); } // bound incluso 
			catch (InterruptedException e) { }
			System.out.println("Visitatore " + id + ": uscito dalla seconda sala.");
		}
	}

	class PersonaTask extends FutureTask<PersonaTask> implements Comparable<PersonaTask>
	{
		private Persona task = null;

		public PersonaTask(Persona task) { super(task, null); this.task = task; }

		public int compareTo(PersonaTask another) { return task.getId() - another.task.getId(); }
	}

	public static void main(String[] args)
	{
		if (args.length != 1) return;

		final int secondaSalaMax;
		final int numSportelli = 4;
		final int timeout = 500;
		final int intervalLength = 150;
		try
		{
			secondaSalaMax = Integer.parseInt(args[0]);
		}
		catch (NumberFormatException e) { return; }

		Facoltativo facoltativo = new Facoltativo();
		PriorityBlockingQueue<Runnable> secondaSalaCoda = new PriorityBlockingQueue<>(secondaSalaMax);
		ThreadPoolExecutor service = new ThreadPoolExecutor(numSportelli, numSportelli, timeout, TimeUnit.MILLISECONDS, secondaSalaCoda);
		service.allowCoreThreadTimeOut(true);
		Random random = new Random();


		int i = 0;
		while (true)
		{
			Persona task = facoltativo.new Persona(i+1);
			try { service.execute(facoltativo.new PersonaTask(task)); }
			catch (RejectedExecutionException r)
			{
				try
				{
					Thread.sleep(50);
					service.execute(facoltativo.new PersonaTask(task));
				}
				catch (Exception e) { System.exit(1); }
			}
			long sleepingTime = random.nextInt(timeout + intervalLength) - intervalLength;
			if (sleepingTime >= timeout) break;
			try { Thread.sleep(sleepingTime); }
			catch (Exception e) { return; }
			i++;
		}
		service.shutdown();
	}
}