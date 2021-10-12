import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.*;

public class Assignment
{
	static final int numComputers = 20;

	static class Laboratory
	{
		static class Person implements Runnable, Comparable<Person>
		{
			public enum Priority
			{
				STUDENT
				{
					public synchronized void useComputer(Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (laboratory.getCurrentlyInUse() == numComputers) this.wait();
						for (int i = 0; i < numComputers; i++)
						{
							if (laboratory.checkIfInUse(i)) continue;
							laboratory.setToBusy(i);
							person.currentIndex = i;
							break;
						}
						System.out.printf("[STUDENT %d] is now in the lab (using %d).\n", person.id, person.currentIndex);
						System.out.flush();
						this.notifyAll();
						return;
					}

					public synchronized void setToFreeComputer(Laboratory laboratory,  Assignment.Laboratory.Person person) throws InterruptedException
					{
						while(!laboratory.checkIfInUse(person.currentIndex)) this.wait();
						laboratory.setToFree(person.currentIndex);
						System.out.printf("[STUDENT %d] is now exiting the lab (freeing %d).\n", person.id, person.currentIndex);
						System.out.flush();
						this.notifyAll();
					}
				},
				THESIST
				{
					public synchronized void useComputer(Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (laboratory.checkIfInUse(person.currentIndex)) this.wait();
						laboratory.setToBusy(person.currentIndex);
						System.out.printf("[THESIST %d] is now in the lab (using %d).\n", person.id, person.currentIndex);
						System.out.flush();
						this.notifyAll();
					}

					public synchronized void setToFreeComputer(Laboratory laboratory,  Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (!laboratory.checkIfInUse(person.currentIndex)) this.wait();
						laboratory.setToFree(person.currentIndex);
						System.out.printf("[THESIST %d] is now exiting the lab (freeing %d).\n", person.id, person.currentIndex);
						System.out.flush();
						this.notifyAll();
					}
				},
				PROFESSOR
				{
					public synchronized void useComputer(Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (laboratory.getCurrentlyInUse() != 0) this.wait();
						for (int i = 0; i < numComputers; i++) laboratory.setToBusy(i);
						System.out.printf("[PROFESSOR %d] is now in the lab.\n", person.id);
						System.out.flush();
						this.notifyAll();
					}

					public synchronized void setToFreeComputer(Laboratory laboratory,  Assignment.Laboratory.Person person) throws InterruptedException
					{
						while (laboratory.getCurrentlyInUse() != numComputers) this.wait();
						for (int i = 0; i < numComputers; i++) laboratory.setToFree(i);
						System.out.printf("[PROFESSOR %d] is now exiting the lab.\n", person.id);
						System.out.flush();
						this.notifyAll();
					}
				};

				public abstract void useComputer(Assignment.Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException;
				public abstract void setToFreeComputer(Assignment.Laboratory laboratory, Assignment.Laboratory.Person person) throws InterruptedException;
			};

			public final int id;
			static final int maxWorkingTime = 100;
			private Priority priority;
			private final Laboratory laboratory;
			private Random random;
			public int currentIndex;


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
					//System.out.println("aa");
					Thread.sleep(random.nextInt(maxWorkingTime));
					laboratory.setToFreeComputer(this);
				}
				catch (Exception e) { }
				return;
			}

			public int compareTo(Person another) { return this.priority.compareTo(another.priority); }
		}

		private boolean[] array = new boolean[numComputers]; // entries are toggled on when in use
		private int count = 0; // number of entries in array currently toggled on

		public Laboratory() { Arrays.fill(array, false); }

		protected void setToFree(int index) { array[index] = false; count--; }

		protected void setToBusy(int index) { array[index] = true; count++; }

		protected boolean checkIfInUse(int index) { return array[index]; }


		protected int getCurrentlyInUse() { return count; }

		public void useComputer(Person person) throws InterruptedException
		{
			person.priority.useComputer(this, person);
		}

		public void setToFreeComputer(Person person) throws InterruptedException
		{
			person.priority.setToFreeComputer(this, person);
		}
	}

	public static ExecutorService createThreadPool(int corePoolSize, int maximumPoolSize, long keepAliveTime, TimeUnit unit, BlockingQueue<Runnable> workQueue)
	{
		ThreadPoolExecutor service = new ThreadPoolExecutor(corePoolSize, maximumPoolSize, keepAliveTime, TimeUnit.MILLISECONDS, workQueue);
		service.allowCoreThreadTimeOut(true);
		return service;
	}

	public static void main(String[] args)
	{
		if (args.length != 3) return;

		final int numStudents, numThesists, numProfessors;
		final int timeout = 500;
		final int intervalLength = 5, upperBound = 15;
		final int accessesNo; // k in the text

		try
		{
			numStudents = Integer.parseInt(args[0]);
			numThesists = Integer.parseInt(args[1]);
			numProfessors = Integer.parseInt(args[2]);
		}
		catch (Exception e) { return; }

		PriorityBlockingQueue<Runnable> workQueue = new PriorityBlockingQueue<>(numStudents + numThesists + numProfessors);
		ExecutorService service = createThreadPool(numComputers, numComputers, timeout, TimeUnit.MILLISECONDS, workQueue);

		Random random = new Random();
		accessesNo = Math.abs(random.nextInt(upperBound + intervalLength) - intervalLength);

		Laboratory laboratory = new Laboratory();
		final int numUsers = numStudents + numThesists + numProfessors;
		Laboratory.Person[] user = new Laboratory.Person[numUsers];
		for (int i = 0; i < numUsers; i++)
		{
			int j = random.nextInt(100);
			switch (j % 3)
			{
				case 0:
					user[i] = new Laboratory.Person(i, Laboratory.Person.Priority.PROFESSOR, laboratory, -1);
					break;
				
				case 1:
					user[i] = new Laboratory.Person(i, Laboratory.Person.Priority.THESIST, laboratory, random.nextInt(numComputers));
					break;

				case 2:
					user[i] = new Laboratory.Person(i, Laboratory.Person.Priority.STUDENT, laboratory, -1);
					break;
			}
		}

		System.out.println(accessesNo + " accesses will now be made.");
		for (int j = 0; j < accessesNo; j++)
		{
			long intervalBetweenAccesses = Math.abs((random.nextInt(upperBound + intervalLength) - intervalLength) * 10);
			for (int i = 0; i < numUsers; i++)
			{
				try
				{
					service.execute(user[i]);
					Thread.sleep(intervalBetweenAccesses);
				}
				catch (RejectedExecutionException | InterruptedException r)
				{
					try
					{
						Thread.sleep(50);
						service.execute(user[i]);
					}
					catch (Exception e) { }
				}
			}
		}
		service.shutdown();
		try { if (!service.awaitTermination(timeout, TimeUnit.MILLISECONDS)) service.shutdownNow(); }
		catch (Exception e) { service.shutdownNow(); }
	}
}