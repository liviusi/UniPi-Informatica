import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.time.Duration;
import java.time.Instant;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.stream.JsonReader;



public class Assignment
{
	public static final int NAME_LENGTH = 10;
	public static final Instant START_INSTANT = Instant.now().minus(Duration.ofDays(2 * 365));
	public static final Instant END_INSTANT = Instant.now();
	public static final int MAX_MOVEMENTS = 10;
	public static final String BANKACCOUNTS_FILENAME = "bankaccounts.json";
	public static final int BUFFER_SIZE = 4096;
	public static final int TIMEOUT = 1500;
	public static final int COREPOOLSIZE = 5;
	public static final int MAXIMUMPOOLSIZE = 20;
	public static final int KEEPALIVETIME = 500;
	public static final String NAME = "name";
	public static final String MOVEMENTS = "movements";
	public static final String DATE = "date";
	public static final String CAUSAL = "causal";

	public static class BankAccount
	{
		public static class BankingMovement
		{
			enum Causal
			{
				Bonifico, Accredito, Bollettino, F24, PagoBancomat
			};

			private static final List<Causal> CAUSALS = Collections.unmodifiableList(Arrays.asList(Causal.values()));
			private static final int CAUSALS_SIZE = CAUSALS.size();
			protected String date;
			protected Causal causal;

			public BankingMovement(Instant date, Causal causal)
			{
				this.date = DateTimeFormatter.ISO_INSTANT.format(date);
				this.causal = causal;
			}

			public static Causal generateRandomCausal()
			{
				return CAUSALS.get(ThreadLocalRandom.current().nextInt(CAUSALS_SIZE));
			}

			@Override
			public String toString()
			{
				return "date:" + date + "causal:" + causal.toString();
			}
		}

		protected String name;
		protected List<BankingMovement> movements;

		public BankAccount(String name)
		{
			this.name = name;
			movements = new LinkedList<>();
		}

		public boolean AddMovement(Instant date, BankingMovement.Causal causal)
		{
			return movements.add(new BankingMovement(date, causal));
		}

		@Override
		public String toString()
		{
			return "name" + name + "movements:" + movements;
		}
	}

	// source: https://docs.oracle.com/javase/tutorial/essential/concurrency/atomicvars.html
	public static class AtomicCounter
	{
		private AtomicInteger c;

		public AtomicCounter() { c = new AtomicInteger(0); }

		public void increment() { c.incrementAndGet(); }

		public void decrement() { c.decrementAndGet(); }

		public int value() { return c.get(); }
	}

	public static class Reader implements Runnable
	{
		BankAccount bankAccount;
		AtomicCounter cBonifico, cAccredito, cBollettino, cF24, cPagoBancomat;

		public Reader(BankAccount bankAccount, AtomicCounter cBonifico, AtomicCounter cAccredito, AtomicCounter cBollettino, AtomicCounter cF24, AtomicCounter cPagoBancomat)
		{
			this.bankAccount = bankAccount;
			this.cBonifico = cBonifico;
			this.cAccredito = cAccredito;
			this.cBollettino = cBollettino;
			this.cF24 = cF24;
			this.cPagoBancomat = cPagoBancomat;
		}

		public void run()
		{
			for (BankAccount.BankingMovement movement : bankAccount.movements)
			{
				switch (movement.causal)
				{
					case Bonifico:
						cBonifico.increment();
						break;

					case Accredito:
						cAccredito.increment();
						break;

					case Bollettino:
						cBollettino.increment();
						break;

					case F24:
						cF24.increment();
						break;

					case PagoBancomat:
						cPagoBancomat.increment();
						break;
				}
			}
			return;
		}
	}

	public static String generateRandomName()
	{
		int a = 97; // letter 'a'
		int z = 122; // letter 'z'
		StringBuilder buffer = new StringBuilder(NAME_LENGTH);
		for (int i = 0; i < NAME_LENGTH; i++)
		{
			int randomLimitedInt = a + (int) (ThreadLocalRandom.current().nextFloat() * (z - a + 1));
			buffer.append((char) randomLimitedInt);
		}
		return buffer.toString();
	}

	public static Instant generateRandomInstant()
	{
		long startSeconds = START_INSTANT.getEpochSecond();
		long endSeconds = END_INSTANT.getEpochSecond();
		long random = ThreadLocalRandom.current().nextLong(startSeconds, endSeconds);
		return Instant.ofEpochSecond(random);
	}

	public static String illegalLabel(String tmp)
	{
		return "Unexpected name met while parsing (" + tmp + "): expected ";
	}

	public static class IllegalLabelException extends Exception
	{
		public IllegalLabelException(String msg)
		{
			super(msg);
		}
	}

	public static class IllegalDateException extends Exception
	{
		public IllegalDateException(String msg)
		{
			super(msg);
		}
	}
	public static void main(String[] args)
	{
		if (args.length != 1)
		{
			System.err.println("Usage: java Assignment <number of bank accounts>");
			System.exit(1);
		}
		int bankAccountsNo = 0;
		try
		{
			bankAccountsNo = Integer.parseInt(args[0]);
			if (bankAccountsNo <= 0) throw new Exception();
		}
		catch (Exception e)
		{
			System.err.println(args[0] + " is not a valid integer.");
			System.exit(1);
		}
		// generating random accounts and movements
		List<BankAccount> bankAccounts = new LinkedList<>();
		for (int i = 0; i < bankAccountsNo; i++)
		{
			BankAccount ba = new BankAccount(generateRandomName());
			final int movementsNo = ThreadLocalRandom.current().nextInt(0, MAX_MOVEMENTS);
			for (int j = 0; j < movementsNo; j++) ba.AddMovement(generateRandomInstant(), BankAccount.BankingMovement.generateRandomCausal());
			bankAccounts.add(ba);
		}

		Gson gson = new GsonBuilder().setPrettyPrinting().create();
		final String json = gson.toJson(bankAccounts);
		// writing list to file
		try
		(
			FileOutputStream fOutputStream = new FileOutputStream(BANKACCOUNTS_FILENAME);
			FileChannel fOutputChannel = fOutputStream.getChannel()
		)
		{
			final ByteBuffer buffer = ByteBuffer.wrap(json.getBytes(Charset.forName("ASCII")));
			fOutputChannel.write(buffer);
		}
		catch (FileNotFoundException e)
		{
			System.err.printf("File could not be found:\n%s", e.getMessage());
			System.exit(1);
		}
		catch (IOException e)
		{
			System.err.printf("I/O error:\n%s", e.getMessage());
			System.exit(1);
		}

		AtomicCounter cBonifico = new AtomicCounter();
		AtomicCounter cAccredito = new AtomicCounter();
		AtomicCounter cBollettino = new AtomicCounter();
		AtomicCounter cF24 = new AtomicCounter();
		AtomicCounter cPagoBancomat = new AtomicCounter();

		final ExecutorService threadPool = new ThreadPoolExecutor(COREPOOLSIZE, MAXIMUMPOOLSIZE, KEEPALIVETIME, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<>());
		// reading file
		try (JsonReader reader = new JsonReader(new InputStreamReader(new FileInputStream(BANKACCOUNTS_FILENAME))))
		{
			reader.beginArray();
			while (reader.hasNext())
			{
				String tmp;
				reader.beginObject();
				tmp = reader.nextName();
				if (!tmp.equals(NAME)) throw new IllegalLabelException(illegalLabel(tmp) + NAME);
				BankAccount bAccount = new BankAccount(reader.nextString());
				tmp = reader.nextName();
				if (!tmp.equals(MOVEMENTS)) throw new IllegalLabelException(illegalLabel(tmp)+ MOVEMENTS);
				reader.beginArray();
				while (reader.hasNext()) // parse the array of movements
				{
					reader.beginObject();
					tmp = reader.nextName();
					if (!tmp.equals(DATE)) throw new IllegalLabelException(illegalLabel(tmp) + DATE);
					final String date = reader.nextString();
					// parsing date to assert it is valid
					Instant i = Instant.parse(date);
					if (Duration.between(i, END_INSTANT).compareTo(Duration.between(START_INSTANT, END_INSTANT)) > 0)
						throw new IllegalDateException("Date " + date + " is more than 2 years old.");
					tmp = reader.nextName();
					if (!tmp.equals(CAUSAL)) throw new IllegalLabelException(illegalLabel(tmp) + CAUSAL);
					tmp = reader.nextString();
					BankAccount.BankingMovement.Causal c = BankAccount.BankingMovement.Causal.valueOf(tmp);
					bAccount.AddMovement(i, c);
					reader.endObject();
				}
				reader.endArray();
				reader.endObject();
				while (true)
				{
					try
					{
						threadPool.execute(new Thread(new Reader(bAccount, cBonifico, cAccredito, cBollettino, cF24, cPagoBancomat)));
						break;
					}
					catch (RejectedExecutionException e) { if (threadPool.isTerminated()) break; }
				}
			}
		}
		catch (FileNotFoundException e)
		{
			System.err.printf("File could not be found:\n%s", BANKACCOUNTS_FILENAME);
			System.exit(1);
		}
		catch (IOException e)
		{
			System.err.printf("I/O error:\n%s", e.getMessage());
			System.exit(1);
		}
		catch (IllegalLabelException e)
		{
			System.err.printf("%s\n", e.getMessage());
			System.exit(1);
		}
		catch (DateTimeParseException e)
		{
			System.err.printf("Date could not be parsed: %s\n", e.getMessage());
			System.exit(1);
		}
		catch (IllegalArgumentException e)
		{
			System.err.printf("Causal could not be parsed: %s\n", e.getMessage());
			System.exit(1);
		}
		catch (IllegalDateException e)
		{
			System.err.printf("%s\n", e.getMessage());
			System.exit(1);
		}

		threadPool.shutdown();
		try { if (!threadPool.awaitTermination(TIMEOUT, TimeUnit.MILLISECONDS)) threadPool.shutdownNow(); }
		catch (Exception e) { threadPool.shutdownNow(); }

		System.out.printf("Bonifico: %d.\nAccredito: %d.\nBollettino: %d.\nF24: %d.\nPagoBancomat: %d.\n", cBonifico.value(),
				cAccredito.value(), cBollettino.value(), cF24.value(), cPagoBancomat.value());
	}
}
