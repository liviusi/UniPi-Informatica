import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class Esercizio3
{
	class Power implements Callable<Double>
	{

		private double base = 0;
		private double exp = 0;

		public Power(double base, double exp)
		{
			this.base = base;
			this.exp = exp;
		}

		public Double call()
		{
			return Math.pow(base, exp);
		}
	}

	public static void main(String[] args)
	{
		if (args.length != 1) return;
		double base = 0;
		try { base = Double.parseDouble(args[0]); }
		catch (NumberFormatException e) { return; }
		Esercizio3 es = new Esercizio3();

		ExecutorService service = Executors.newCachedThreadPool();
		List<Future<Double>> results = new ArrayList<Future<Double>>(49);
		for (int i = 2; i <= 50; i++)
		{
			try { results.add(i - 2, service.submit(es.new Power(base, i))); }
			catch (RejectedExecutionException e) { return; }
		}
		double sum = 0;
		for (int i = 0; i < 49; i++)
		{
			try { sum += results.get(i).get(1000L, TimeUnit.MILLISECONDS); }
			catch (Exception e) { return; }
			System.out.println("sum = " + sum);
		}
		try { service.shutdown(); }
		catch (Exception e) { System.exit(1); }
	}
}