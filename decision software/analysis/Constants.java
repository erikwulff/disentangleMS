package analysis;

public class Constants {

	public static double H1Mass = 1.007276;
	public static double O16Mass = 15.99491461956;
	public static double O18Mass = 17.9991610;
	public static double waterMass = O16Mass + 2 * H1Mass;
	public static double O18WaterMass = O18Mass + 2 * H1Mass;
	public static double C12Mass = 12;
	public static double C13Mass = 13.0033548378;
	public static double C12C13Difference = C13Mass - C12Mass;
	
	public static double C13Probability = 0.0107;
	public static double C12Probability = 1 - C13Probability;
	public static double carbonFrequency = 0.044;
	
}
