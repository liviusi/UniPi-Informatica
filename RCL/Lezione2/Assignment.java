import java.util.Queue;
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
		if (args.length != 3) return;

		final int primaSalaMax; // capienza prima sala
		final int secondaSalaMax; // capienza seconda sala (i.e. k)
		final int numSportelli = 4;
		final int numVisitatori; // necessario per assicurare la terminazione
		try
		{
			primaSalaMax = Integer.parseInt(args[0]);
			secondaSalaMax = Integer.parseInt(args[1]);
			numVisitatori = Integer.parseInt(args[2]);
		}
		catch (NumberFormatException e) { return; }

		Assignment assignment = new Assignment();
		Queue<Integer> primaSalaCoda = new LinkedBlockingQueue<Integer>(primaSalaMax);
		PriorityBlockingQueue<Runnable> secondaSalaCoda = new PriorityBlockingQueue<>(secondaSalaMax);
		ExecutorService service = new ThreadPoolExecutor(numSportelli, numSportelli, 5000, TimeUnit.MILLISECONDS, secondaSalaCoda);

		int i = 0;
		while (i < numVisitatori)
		{
			if (primaSalaCoda.isEmpty()) // riempio la coda
				for (int j = i; primaSalaCoda.offer(j+1); j++) i++;
			while (!primaSalaCoda.isEmpty()) // svuoto la coda
			{
				Persona task = assignment.new Persona(primaSalaCoda.remove());
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
		}
		service.shutdown();
	}
}