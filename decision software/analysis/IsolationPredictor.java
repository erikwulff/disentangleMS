package analysis;
import data_structures.Peak;
import data_structures.Spectrum;
import data_structures.Variants;


public class IsolationPredictor {

	Spectrum predictSingleVariant(Peak peak, double isolationWidth){
		Variants originalVariants = peak.getVariants();
		Spectrum isolatedSpectrum = new Spectrum();
		Variants isolatedVariants = new Variants(isolatedSpectrum);
		for(Peak originalPeak : originalVariants){
			double mzOffset = originalPeak.getMZ() - peak.getMZ();
			double isolationEfficiency = Math.pow(Math.E, - Math.pow(mzOffset, 2) / Math.pow(isolationWidth, 2));
			if(isolationEfficiency > 0.005){
				double isolatedIntensity = originalPeak.getIntensity() * isolationEfficiency;
				Peak isolatedPeak = new Peak(originalPeak.getMZ(), isolatedIntensity);
				isolatedPeak.setCharge(originalPeak.getCharge());
				isolatedPeak.setIsotopeNumber(originalPeak.getIsotopeNumber());
				isolatedVariants.add(isolatedPeak);	
			}
		}
		isolatedSpectrum.add(isolatedVariants);
		return isolatedSpectrum;
	}
	
}
