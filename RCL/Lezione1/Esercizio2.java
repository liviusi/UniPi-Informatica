import java.text.SimpleDateFormat;
import java.util.Calendar;


public class Esercizio2
{
	class DatePrinterThread extends Thread
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
		Esercizio2 tmp = new Esercizio2();
		DatePrinterThread dpt = tmp.new DatePrinterThread();
		dpt.start();
	}
}