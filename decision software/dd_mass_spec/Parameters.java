package dd_mass_spec;

public class Parameters {
	public static double accuracy = 0.05;			// mz units
	public static double runTime = 60 * 60;			// seconds
	public static double dutyCycleLength = 3.0;		// seconds
	
	public static double deconvolutionLimit = 100;	// ms
	public static int maxIsotope = 7;
	public static int maxCharge = 6;
	
	public static int maxScansOnInstrument = 3;
	public static int maxScansForInstrument = 3;
}
