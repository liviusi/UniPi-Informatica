import java.util.concurrent.*;

/*
Simulare il flusso di clienti in un ufficio postale che ha 4 sportelli. Nell'ufficio esiste:

un'ampia sala d'attesa in cui ogni persona può entrare liberamente. Quando entra, ogni persona prende il numero dalla numeratrice e aspetta il proprio
turno in questa sala.
una seconda sala, meno ampia, posta davanti agli sportelli, in cui si può entrare solo a gruppi di k persone
una persona si mette quindi prima in coda nella prima sala, poi passa nella seconda sala.
Ogni persona impiega un tempo differente per la propria operazione allo sportello. Una volta terminata l'operazione, la persona esce dall'ufficio.

Scrivere un programma in cui:

l'ufficio viene modellato come una classe JAVA, in cui viene attivato un ThreadPool di dimensione uguale al numero degli sportelli
la coda delle persone presenti nella sala d'attesa è gestita esplicitamente dal programma
la seconda coda (davanti agli sportelli) è quella gestita implicitamente dal ThreadPool
ogni persona viene modellata come un task, un task che deve essere assegnato ad uno dei thread associati agli sportelli
si preveda di far entrare tutte le persone nell'ufficio postale, all'inizio del programma
Facoltativo: prevedere il caso di un flusso continuo di clienti e la possibilità che l'operatore chiuda lo sportello stesso dopo che in
un certo intervallo di tempo non si presentano clienti al suo sportello.
*/

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