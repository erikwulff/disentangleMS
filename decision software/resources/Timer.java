package resources;


public class Timer {
	long startTime;
	static private long universalStartTime;
	
	public Timer(){
		reset();
	}
	
	public Timer(double seconds){
		setSeconds(seconds);
	}
	
	public double ms(){
		return System.currentTimeMillis() - startTime;
	}
	
	public double seconds(){
		return ms() / 1000.0;
	}
	
	public double minutes(){
		return seconds() / 60.0;
	}
	
	public void reset(){
		startTime = System.currentTimeMillis();
	}
	
	public void setMS(long ms){
		startTime = System.currentTimeMillis() - ms;
	}
	
	public void setSeconds(double seconds){
		startTime = System.currentTimeMillis() - (long) (1000 * seconds);
	}
	
	public void subtractMS(long ms){
		startTime = Math.min(System.currentTimeMillis(), startTime + ms);
	}
	
	public static void resetUniversalTime(){
		universalStartTime = System.currentTimeMillis();
	}
	
	public static long universalMS(){
		return System.currentTimeMillis() - universalStartTime;
	}
}
