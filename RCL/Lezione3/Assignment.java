import java.util.Random;
import java.util.concurrent.*;
import java.util.concurrent.locks.*;

public class Assignment
{

	static final int numComputers = 20;

	public enum Priority
	{
		STUDENT, THESIST, PROFESSOR
	};

	public static class Laboratory
	{
		public final ReadWriteLock laboratoryLock;
		public final Lock[] computerLock = new Lock[numComputers];
		public final Condition[] computerLockCondition = new Condition[numComputers];
		private boolean[] availableComputer = new boolean[numComputers];

		public Laboratory()
		{
			laboratoryLock = new ReentrantReadWriteLock();
			for (int i = 0; i < numComputers; i++)
			{
				computerLock[i] = new ReentrantLock();
				computerLockCondition[i] = computerLock[i].newCondition();
				availableComputer[i] = true;
			}
		}

		// to choose a random index for the lock the user should wait for, a hash function over the given interval
		// is used as it guarantees a normal distribution.
		public int IDtoComputerIndex(Thread requestor) { return Math.abs(requestor.hashCode() % numComputers); }

		public boolean isAvailable(int index) { return availableComputer[index]; }

		public void assignComputer(int index) { availableComputer[index] = false; }

		public void releaseComputer(int index) { if (index < availableComputer.length) availableComputer[index] = true; }
	}

	public static class Person implements Runnable, Comparable<Person>
	{
		private final Priority priority; // student, thesist or professor
		private final Laboratory laboratory;
		private final long workingTime; // time to be spent sleeping

		public Person(Priority priority, Laboratory laboratory, long workingTime)
		{
			this.priority = priority;
			this.laboratory = laboratory;
			this.workingTime = workingTime;
		}

		public Priority getPriority() { return priority; }

		public void run()
		{
			int index = Integer.MIN_VALUE; // index of the computer this thread has been assigned
			if (this.priority == Priority.PROFESSOR)
			{
				try
				{
					// professors need to access lock in writer mode as they need every resource
					laboratory.laboratoryLock.writeLock().lock();
					System.out.printf("[PROFESSOR %d] is now in the lab.\n", Thread.currentThread().getId());
					try { Thread.sleep(workingTime); } // work
					catch (Exception e) { } // not to be handled
				}
				// release resources
				finally { laboratory.laboratoryLock.writeLock().unlock(); }
				System.out.printf("[PROFESSOR %d] is not in the lab anymore.\n", Thread.currentThread().getId());
			}
			else
			{
				try
				{
					// get room's lock as a reader
					laboratory.laboratoryLock.readLock().lock();
					System.out.printf("[S/T %d] is now in the lab.\n", Thread.currentThread().getId());
					// hash function is called to choose a random index
					index = laboratory.IDtoComputerIndex(Thread.currentThread());
					if (index >= numComputers || index < 0) System.exit(1); // fatal error
					while (!laboratory.isAvailable(index))
					{ laboratory.computerLockCondition[index].awaitUninterruptibly(); } // wait for a computer to be available
					// a computer is now available
					System.out.printf("[S/T %d] is now being assigned computer no. %d.\n", Thread.currentThread().getId(), index);
					try
					{
						laboratory.computerLock[index].lock();
						try { Thread.sleep(workingTime); }
						catch (Exception e) { }
					}
					finally
					{
						laboratory.releaseComputer(index);
						System.out.printf("[S/T %d] is now releasing computer no. %d.\n", Thread.currentThread().getId(), index);
						laboratory.computerLockCondition[index].signal();
						laboratory.computerLock[index].unlock();
					}
				}
				finally { laboratory.laboratoryLock.readLock().unlock(); }
			}
		}

		public int compareTo(Assignment.Person another)
		{
			return this.priority.compareTo(another.priority);
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
		Person[] user = new Person[numUsers];
		for (int i = 0; i < numUsers; i++)
		{
			long workingTime = Math.abs((random.nextInt(upperBound + intervalLength) - intervalLength) * 10);
			switch (i % 3)
			{
				case 0:
					user[i] = new Person(Priority.PROFESSOR, laboratory, workingTime);
					break;

				case 1:
					user[i] = new Person(Priority.THESIST, laboratory, workingTime);
					break;

				case 2:
					user[i] = new Person(Priority.STUDENT, laboratory, workingTime);
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
