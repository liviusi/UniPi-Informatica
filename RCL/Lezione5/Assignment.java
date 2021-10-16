import java.io.File;
import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;

public class Assignment
{

	static AtomicBoolean noMoreDirectories = new AtomicBoolean(false);
	static AtomicBoolean isPrinting = new AtomicBoolean(false);

	static class Producer implements Runnable
	{
		private final File root;
		private SynchLinkedList<File> fList;

		public Producer(File root, SynchLinkedList<File> fList)
		{
			this.root = root;
			this.fList = fList;
		}

		private static void rec_listDirectories(File f, SynchLinkedList<File> fList)
		{
			if (!f.isDirectory()) return;
			else fList.push(f);
			for (File i : f.listFiles()) rec_listDirectories(i, fList);
			return;
		}

		public void run()
		{
			rec_listDirectories(root, fList);
			noMoreDirectories.set(true);
		}
	}

	static class Consumer implements Runnable
	{
		final static long waitTimeout = 500;
		private SynchLinkedList<File> fList;


		public Consumer(SynchLinkedList<File> fList)
		{
			this.fList = fList;
		}

		private static List<File> listFiles(List<File> listFiles, File f)
		{
			for (File i : f.listFiles())
			{
				if (i.isDirectory()) continue;
				listFiles.add(i);
			}
			return listFiles;
		}

		public synchronized void run()
		{
			File dir = fList.poll();
			if (dir == null && noMoreDirectories.get()) return;
			List<File> lFiles = new LinkedList<>();
			listFiles(lFiles, dir);
			while (isPrinting.get())
			{
				try { this.wait(waitTimeout); }
				catch (InterruptedException e) { }
			}
			isPrinting.set(true);
			try { System.out.println(dir.getCanonicalPath() + " contains:"); }
			catch (IOException e) { }
			for (File f : lFiles)
			{
				try { System.out.println("-\t" + f.getCanonicalPath()); }
				catch (IOException e) { }
			}
			System.out.flush();
			isPrinting.set(false);
			this.notifyAll();
		}
	}

	static class SynchLinkedList<T>
	{
		static long waitTimeout = 50;
		private LinkedList<T> list;
		private int size = 0;

		public SynchLinkedList()
		{
			this.list = new LinkedList<>();
		}

		public T poll()
		{
			T object;
			synchronized(this)
			{
				while (size == 0)
				{
					try { this.wait(waitTimeout); }
					catch (InterruptedException e) { }
					if (noMoreDirectories.get()) return null;
				}
				object = list.poll();
				size--;
			}
			return object;
		}

		public void push(T object)
		{
			synchronized(this)
			{
				list.offer(object);
				size++;
				this.notifyAll();
			}
			return;
		}
	}

	public static void main(String[] args)
	{
		if (args.length != 1)
		{
			System.err.println("java Assignment </path/to/dir>");
			return;
		}

		final int k = 10;
		File root = new File(args[0]);
		if (!root.isDirectory())
		{
			System.err.println("java Assignment </path/to/dir>");
			return;
		}

		SynchLinkedList<File> fList = new SynchLinkedList<>();
		Thread producer = new Thread(new Producer(root, fList));
		Thread consumer[] = new Thread[k];
		for (int i = 0; i < k; i++) consumer[i] = new Thread(new Consumer(fList));
		producer.start();
		for (int i = 0; i < k; i++)
		{
			consumer[i].start();
		}
		try
		{
			producer.join();
			for (int i = 0; i < k; i++) consumer[i].join();
		}
		catch (InterruptedException e) { }
	}
}
