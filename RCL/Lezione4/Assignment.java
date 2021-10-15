import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class Assignment
{
	static final int numComputers = 20;
	static final long waitTimeout = 500;

	static class Laboratory
	{
		static class Person implements Runnable
		{
			public enum Priority
			{
				STUDENT
				{
					public synchronized void useComputer(Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (laboratory.count == numComputers) this.wait(waitTimeout);
						for (int i = 0; i < numComputers; i++)
						{
							if (laboratory.array[i]) continue;
							laboratory.count++;
							laboratory.array[i] = true;
							person.currentIndex = i;
							break;
						}
						System.out.printf("[STUDENT %d] is now in the lab (using %d).\n", person.id, person.currentIndex);
						// System.out.flush();
						this.notifyAll();
						return;
					}

					public synchronized void freeComputer(Laboratory laboratory,  Assignment.Laboratory.Person person) throws InterruptedException
					{
						while(!laboratory.array[person.currentIndex]) this.wait(waitTimeout);
						laboratory.array[person.currentIndex] = false;
						laboratory.count--;
						System.out.printf("[STUDENT %d] is now exiting the lab (freeing %d).\n", person.id, person.currentIndex);
						// System.out.flush();
						this.notifyAll();
					}
				},

				THESIST
				{
					public synchronized void useComputer(Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (laboratory.array[person.currentIndex]) this.wait(waitTimeout);
						laboratory.array[person.currentIndex] = true;
						laboratory.count++;
						System.out.printf("[THESIST %d] is now in the lab (using %d).\n", person.id, person.currentIndex);
						// System.out.flush();
						this.notifyAll();
					}

					public synchronized void freeComputer(Laboratory laboratory,  Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (!laboratory.array[person.currentIndex]) this.wait(waitTimeout);
						laboratory.array[person.currentIndex] = false;
						laboratory.count--;
						System.out.printf("[THESIST %d] is now exiting the lab (freeing %d).\n", person.id, person.currentIndex);
						// System.out.flush();
						this.notifyAll();
					}
				},

				PROFESSOR
				{
					public synchronized void useComputer(Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (laboratory.count != 0) this.wait(waitTimeout);
						Arrays.fill(laboratory.array, true);
						laboratory.count = 20;
						System.out.printf("[PROFESSOR %d] is now in the lab.\n", person.id);
						// System.out.flush();
						this.notifyAll();
					}

					public synchronized void freeComputer(Laboratory laboratory,  Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (laboratory.count != numComputers) this.wait(waitTimeout);
						Arrays.fill(laboratory.array, false);
						laboratory.count = 0;
						System.out.printf("[PROFESSOR %d] is now exiting the lab.\n", person.id);
						// System.out.flush();
						this.notifyAll();
					}
				};

				public abstract void useComputer(Assignment.Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException;
				public abstract void freeComputer(Assignment.Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException;
			};

			public final int id;
			static final int maxWorkingTime = 1000, minWorkingTime = 200;
			private Priority priority;
			private final Laboratory laboratory;
			private Random random;
			private int currentIndex;


			public Person(int id, Priority priority, Laboratory laboratory, int index)
			{
				this.id = id;
				this.priority = priority;
				if (priority == Priority.THESIST) this.currentIndex = index;
				else this.currentIndex = -1;
				this.laboratory = laboratory;
				random = new Random();
			}

			public Priority getPriority() { return this.priority; }

			public void run()
			{
				try
				{
					laboratory.useComputer(this);
					Thread.sleep(random.nextInt(maxWorkingTime - minWorkingTime) + minWorkingTime);
					laboratory.freeComputer(this);
				}
				catch (Exception e) { }
				return;
			}
		}

		private boolean[] array = new boolean[numComputers]; // array[i] is toggled on when the i-th computer is in use
		private int count; // number of entries in array currently toggled on

		public Laboratory() { Arrays.fill(array, false); count = 0; }

		public void useComputer(Person person) throws InterruptedException { person.priority.useComputer(this, person); }

		public void freeComputer(Person person) throws InterruptedException { person.priority.freeComputer(this, person); }
	}

	// instantiate one thread per user
	public static List<Thread> instantiateThreads(int numStudents, int numThesists, int numProfessors, Laboratory laboratory, Random random)
	{
		List<Thread> userThread = new ArrayList<>(numStudents + numThesists + numProfessors);
		for (int i = 0; i < numProfessors; i++)
			userThread.add(i, new Thread(new Laboratory.Person(i, Laboratory.Person.Priority.PROFESSOR, laboratory, -1)));
		for (int i = 0; i < numThesists; i++)
			userThread.add(i + numProfessors, new Thread(new Laboratory.Person(i, Laboratory.Person.Priority.THESIST, laboratory, random.nextInt(numComputers))));
		for (int i = 0; i < numStudents; i++)
			userThread.add(i + numProfessors + numThesists, new Thread(new Laboratory.Person(i, Laboratory.Person.Priority.STUDENT, laboratory, -1)));
		return userThread;
	}

	public static void main(String[] args)
	{
		if (args.length != 3) { System.err.println("java Assignment <numStudents> <numThesists> <numProfessors>"); return; }

		final int numStudents, numThesists, numProfessors;
		final int joinTimeout = 500, maxIntervalBetweenAccesses = 200;
		final int minAccesses = 3, maxAccesses = 5;
		final int accessesNo; // k in the text
		Random random = new Random();
		Laboratory laboratory = new Laboratory();
		List<Thread> userThread;

		try
		{
			numStudents = Integer.parseInt(args[0]);
			numThesists = Integer.parseInt(args[1]);
			numProfessors = Integer.parseInt(args[2]);
		}
		catch (Exception e) { System.err.println("java Assignment <numStudents> <numThesists> <numProfessors>"); return; }

		accessesNo = random.nextInt(maxAccesses - minAccesses) + minAccesses;
		System.out.println(accessesNo + " accesses will now be made.");

		for (int i = 0; i < accessesNo; i++)
		{
			userThread = instantiateThreads(numStudents, numThesists, numProfessors, laboratory, random);
			for (Thread t : userThread)
			{
				t.start();
				try { Thread.sleep(maxIntervalBetweenAccesses); }
				catch (Exception e) { }
			}
			for (Thread t : userThread)
			{
				try { t.join(joinTimeout); }
				catch (Exception e) { }
			}
		}

	}
}