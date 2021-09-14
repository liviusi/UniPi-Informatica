import java.text.SimpleDateFormat;
import java.util.Calendar;

public class Esercizio1
{
	public static void main(String[] args)
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