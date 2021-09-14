import java.text.SimpleDateFormat;
import java.util.Calendar;

public class Esercizio3
{
	class DatePrinterRunnable implements Runnable
	{
		public void run()
		{
			SimpleDateFormat formatter = new SimpleDateFormat("dd-MM-yyyy HH:mm:ss");
			while(true)
			{
				Calendar calendar = Calendar.getInstance();
				System.out.println(Thread.currentThread().getName() + " - " + formatter.format(calendar.getTime()));
				try { Thread.sleep(2000); }
				catch (InterruptedException ie) { return; }
			}
		}
	}

	public static void main(String[] args)
	{
		Esercizio3 tmp = new Esercizio3();
		Thread t = new Thread(tmp.new DatePrinterRunnable());
		t.start();
		System.out.println(Thread.currentThread().getName());
	}
}
