public class Assignment
{

	class GregoryLeibniz implements Runnable
	{
		private double accuracy;
		private double pi;
		private int step;

		public GregoryLeibniz(double accuracy)
		{
			this.accuracy = accuracy;
			this.pi = 0;
			this.step = 0;
		}

		public double getPi() { return this.pi; }

		public int getStep() { return this.step; }

		public void run()
		{
			int sign = 1;
			while(true)
			{
				pi += sign * (4.0 / (step * 2 + 1));
				sign *= -1;
				step += 1;
				if (Thread.currentThread().isInterrupted())
				{
					System.out.println("TIME'S UP!\nAccuracy reached = " + Math.abs(pi - Math.PI));
					break;
				}
				if (Math.abs(pi - Math.PI) <= accuracy)
				{
					System.out.println("ACCURACY THERSHOLD REACHED!\nAccuracy = " + accuracy + " - Reached: " + Math.abs(pi - Math.PI));
					break;
				}
			}
		}
	}

	public static void main(String[] args)
	{
		// ARGS[0] -> ACCURACY
		// ARGS[1] -> TIMEOUT
		if (args.length != 2) return;
		double accuracy; int timeout;
		try { accuracy = Double.parseDouble(args[0]); timeout = Integer.parseInt(args[1]); }
		catch (NumberFormatException e) { return; }
		Assignment tmp = new Assignment();
		GregoryLeibniz gl = tmp.new GregoryLeibniz(accuracy);
		Thread t = new Thread(gl);
		t.start();
		try { Thread.sleep(0, timeout); }
		catch (InterruptedException e) { return; }
		t.interrupt();
		System.out.println("ESTIMATED PI VALUE = " + gl.getPi() + " - STEPS TAKEN: " + gl.getStep());
	}
}
