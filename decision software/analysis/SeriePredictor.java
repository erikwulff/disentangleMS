package analysis;
import java.util.Arrays;

import resources.Library;
import data_structures.Peak;
import data_structures.Spectrum;
import data_structures.Variants;


public class SeriePredictor {
	int NUMBER_OF_CHARGES = 6;
	int NUMBER_OF_ISOTOPES = 7;
	double[][] envelopes = new double[NUMBER_OF_CHARGES * 2000 / 50][NUMBER_OF_ISOTOPES];
	IsolationPredictor isolationPredictor = new IsolationPredictor();
	
	public SeriePredictor(double c13Probability, double carbonFrequency){
		for(int i = 0; i < NUMBER_OF_CHARGES * 2000 / 50; i++){
			double mass = i * 50;
			int numberOfCarbons = (int) (carbonFrequency * mass);
			double c12Probability = 1 - c13Probability;
	
			for(int x = 0; x < NUMBER_OF_ISOTOPES; x++){
				double binomialCoefficient = Library.binomial_coefficient(numberOfCarbons, x);
				double p1 = Math.pow(c13Probability, x);
				double p2 = Math.pow(c12Probability, numberOfCarbons - x);
				envelopes[i][x] = binomialCoefficient * p1 * p2;
			}
		}
	}
	
	public Spectrum predict(Peak peak, int charge, int isotopeNumber){
		if(peak.getSpectrum().scanMode == 0 || peak.getSpectrum().getParentPeak() == null 
				|| peak.getSpectrum().getParentPeak().getVariants() == null)
			return MS1(peak, charge, isotopeNumber);
		else
			return MSn(peak, charge, isotopeNumber);
	}
	
	private Spectrum MS1(Peak peak, int charge, int isotopeNumber){
		double baseMass = peak.getMZ() * charge - charge * Constants.H1Mass - isotopeNumber * Constants.C12C13Difference;
		int lookUp = (int) (baseMass / 50);
		if(envelopes[lookUp][isotopeNumber] < 0.002)
			return null;
		Spectrum spectrum = new Spectrum();
		Variants variants = new Variants(spectrum);
		double scaleFactor = peak.getIntensity() / envelopes[lookUp][isotopeNumber];
		for(int i = 0; i < NUMBER_OF_ISOTOPES; i++){
			double mz = (baseMass + i * Constants.C12C13Difference + charge * Constants.H1Mass) / charge;
			double intensity = envelopes[lookUp][i] * scaleFactor;
			if(intensity > 10.0){
				Peak newPeak = new Peak(mz, intensity);
				newPeak.setIsotopeNumber(i);
				newPeak.setCharge(charge);
				spectrum.add(newPeak);
				variants.add(newPeak);
			}
		}
		return spectrum;
	}
	
	private Spectrum MSn(Peak peak, int charge, int isotopeNumber){
		Spectrum spectrum = peak.getSpectrum();
		Peak parentPeak = spectrum.getParentPeak();
		double isolationWidth = spectrum.isolationWidth;
		double baseMass = peak.getMZ() * charge - charge * Constants.H1Mass - isotopeNumber * Constants.C12C13Difference;
		double parentBaseMass = parentPeak.getVariants().getBaseMass();
		if(baseMass > parentBaseMass || charge > parentPeak.getCharge())
			return null;
		Spectrum isolated = isolationPredictor.predictSingleVariant(parentPeak, isolationWidth);
	
		double[] sums = new double[NUMBER_OF_ISOTOPES];
		Arrays.fill(sums, 0);
		for( Peak p : isolated )
			for(int i = 0; i < p.getIsotopeNumber(); i++)
				sums[i] += p.getIntensity() * Library.binomial(baseMass / parentBaseMass, p.getIsotopeNumber(), i);
		
		double scaleFactor = peak.getIntensity() / sums[isotopeNumber];
		Spectrum newSet = new Spectrum();
		Variants newVariants = new Variants(newSet);
		for( int i = 0; i < NUMBER_OF_ISOTOPES; i++ )
			if(sums[i] * scaleFactor > 10.0){
				double mz = (baseMass + i * Constants.C12C13Difference + charge * Constants.H1Mass) / charge;
				Peak p = new Peak(mz , sums[i] * scaleFactor);
				p.setIsotopeNumber(i);
				p.setCharge(charge);
				newSet.add(p);
				newVariants.add(p);
			}
		
		return newSet;
	}
	
	public Spectrum predict(double baseMass, int charge, Spectrum spectrum){
		if(spectrum.scanMode == 0)
			return MS1(baseMass, charge);
		else
			return MSn(baseMass, charge, spectrum);
	}
	
	private Spectrum MS1(double baseMass, int charge){
		Spectrum spectrum = new Spectrum();
		Variants variants = new Variants(spectrum);
		int lookUp = (int) (baseMass / 50);
		for(int i = 0; i < NUMBER_OF_ISOTOPES; i++){
			double mz = (baseMass + i * Constants.C12C13Difference + charge * Constants.H1Mass) / charge;
			double intensity = envelopes[lookUp][i];
			if(intensity > 0.002){
				Peak peak = new Peak(mz, intensity);
				peak.setIsotopeNumber(i);
				peak.setCharge(charge);
				spectrum.add(peak);
				variants.add(peak);
			}
		}
		return spectrum;
	}
	
	private Spectrum MSn(double baseMass, int charge, Spectrum spectrum){
		Peak parentPeak = spectrum.getParentPeak();
		double isolationWidth = spectrum.isolationWidth;
		double parentBaseMass = parentPeak.getVariants().getBaseMass();
		if(baseMass > parentBaseMass || charge > parentPeak.getCharge())
			return null;
		Spectrum isolated = isolationPredictor.predictSingleVariant(parentPeak, isolationWidth);
		
		double[] sums = new double[NUMBER_OF_ISOTOPES];
		Arrays.fill(sums, 0);
		for( Peak p : isolated )
			for(int i = 0; i < p.getIsotopeNumber(); i++)
				sums[i] += p.getIntensity() * Library.binomial(baseMass / parentBaseMass, p.getIsotopeNumber(), i);
		
		Spectrum newSet = new Spectrum();
		Variants newVariants = new Variants(newSet);
		for( int i = 0; i < NUMBER_OF_ISOTOPES; i++ )
			if(sums[i] > 0.0){
				double mz = (baseMass + i * Constants.C12C13Difference + charge * Constants.H1Mass) / charge;
				Peak p = new Peak(mz , sums[i]);
				p.setIsotopeNumber(i);
				p.setCharge(charge);
				newSet.add(p);
				newVariants.add(p);
			}
		
		return newSet;
	}

}
