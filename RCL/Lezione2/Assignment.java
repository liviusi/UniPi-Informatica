import java.util.concurrent.*;

public class Assignment
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
		if (args.length != 2) return;

		final int k;
		final int visitatori;
		final int sportelli = 4;
		try
		{
			k = Integer.parseInt(args[0]);
			visitatori = Integer.parseInt(args[1]);
		}
		catch (NumberFormatException e) { return; }

		Assignment assignment = new Assignment();
		PriorityBlockingQueue<Runnable> queue = new PriorityBlockingQueue<>(k);
		ExecutorService service = new ThreadPoolExecutor(sportelli, sportelli, 5000, TimeUnit.MILLISECONDS, queue);


		for (int i = 0; i < visitatori; i++)
		{
			Persona task = assignment.new Persona(i+1);
			try { service.execute(assignment.new PersonaTask(task)); }
			catch (RejectedExecutionException r)
			{
				try
				{
					Thread.sleep(50);
					service.execute(assignment.new PersonaTask(task));
				}
				catch (Exception e) { System.exit(1); }
			}
		}
		service.shutdown();
	}
}