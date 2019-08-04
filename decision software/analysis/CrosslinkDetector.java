package analysis;
import data_structures.Peak;
import data_structures.PeakSet;
import data_structures.Spectrum;
import data_structures.Variants;

public class CrosslinkDetector {

	Variants dhaPeaks = null;
	Variants sulfenicPeaks = null;
	Variants thialPeaks = null;
	
	public boolean findCrosslink(Spectrum spectrum){
		dhaPeaks = null;
		sulfenicPeaks = null;
		thialPeaks = null;
			
		if(spectrum.getParentPeak().getVariants() == null)
			return false;
		findBestAnnotation(spectrum);
		if(dhaPeaks == null && sulfenicPeaks == null && thialPeaks == null)
			return false;
		if(isCrosslink(spectrum)){
			for(Peak p : dhaPeaks)
				p.color = "blue";
			for(Peak p : sulfenicPeaks)
				p.color = "green";
			for(Peak p : thialPeaks)
				p.color = "red";
			spectrum.getParentPeak().color = "blue";
			return true;
		}
		return false;
	}
	
	private void findBestAnnotation(Spectrum spectrum){
		double parentMass = spectrum.getParentPeak().getVariants().getBaseMass();
		int parentCharge = spectrum.getParentPeak().getCharge();
		double parentMZ = spectrum.parentMass;
		double bestScore = -1;
		
		for(Variants variants : spectrum.variantsList){
			double dhaMass = variants.getBaseMass();
			double sulfenicMass = parentMass - dhaMass;
			double thialMass = sulfenicMass - Constants.O18WaterMass;
			
			Variants dhaVariants = variants;
			Variants sulfenicVariants = spectrum.getVariantsNearest(sulfenicMass);
			Variants thialVariants = spectrum.getVariantsNearest(thialMass);
			if(dhaVariants == null || sulfenicVariants == null || thialVariants == null)
				continue;
			
			double score = 0;
			for(int q = 1; q < parentCharge; q++){
				PeakSet dhas = dhaVariants.getPeaksWithCharge(q);
				PeakSet sulfenics = sulfenicVariants.getPeaksWithCharge(parentCharge - q);
				PeakSet thials = thialVariants.getPeaksWithCharge(parentCharge - q);
				
				if(dhas.isEmpty() || sulfenics.isEmpty() || thials.isEmpty())
					continue;
				if(parentMZ-20 < dhas.getFirstPeakByMZ().getMZ() && dhas.getFirstPeakByMZ().getMZ() < parentMZ+5)
					continue;
				if(parentMZ-20 < sulfenics.getFirstPeakByMZ().getMZ() && sulfenics.getFirstPeakByMZ().getMZ() < parentMZ+5)
					continue;
				if(parentMZ-20 < thials.getFirstPeakByMZ().getMZ() && thials.getFirstPeakByMZ().getMZ() < parentMZ+5)
					continue;
				
				score += dhas.getTotalIntensity() + sulfenics.getTotalIntensity() + thials.getTotalIntensity();
			}
			if(score > bestScore ){
				bestScore = score;
				dhaPeaks = dhaVariants;
				sulfenicPeaks = sulfenicVariants;
				thialPeaks = thialVariants;
			}
		}
	}
	
	private boolean isCrosslink(Spectrum spectrum){
		int parentCharge = spectrum.getParentPeak().getCharge();
		double parentMZ = spectrum.parentMass;
		
		double intensityTotal = spectrum.getTotalIntensity();
		double intensityMatch = dhaPeaks.getTotalIntensity() + 
								sulfenicPeaks.getTotalIntensity() + 
								thialPeaks.getTotalIntensity();
		double intensityNonMatch = intensityTotal - intensityMatch;
		double intensityCommon = spectrum.getPeaksInRange(parentMZ - 20, parentMZ + 5).getTotalIntensity();
		double intensityOther = intensityTotal - intensityMatch - intensityCommon;
		
		if(intensityOther != 0 && intensityMatch / intensityOther < 1/10)
			return false;
		if(intensityNonMatch != 0 && intensityMatch / intensityNonMatch < 1/20)
			return false;
		
		for(int q = 1; q < parentCharge; q++){
			double dhaIntensity = dhaPeaks.getPeaksWithCharge(q).getTotalIntensity();
			double sulfenicIntensity = sulfenicPeaks.getPeaksWithCharge(parentCharge - q).getTotalIntensity();
			double thialIntensity = sulfenicPeaks.getPeaksWithCharge(parentCharge - q).getTotalIntensity();
			if((dhaIntensity + sulfenicIntensity + thialIntensity) > intensityTotal / 10)
				if(sulfenicIntensity > dhaIntensity / 15 && dhaIntensity > sulfenicIntensity / 15)
					if(thialIntensity > sulfenicIntensity / 25 && sulfenicIntensity > thialIntensity / 10)
						return true;
		}
		return false;
	}
	
	public Peak getDhaPeak(){
		double bestScore = 0;
		Peak bestPeak = null;
		int[] dhaMultipliers = {0, 100, 2000, 100, 10, 1, 1, 1, 1, 1};
		for(Peak peak : dhaPeaks){
			int charge = peak.getCharge();
			double intensity = peak.getIntensity();
			double score = intensity * dhaMultipliers[charge];
			if(score > bestScore){
				bestScore = score;
				bestPeak = peak;
			}
		}
		return bestPeak;
	}
	
	public Peak getThialPeak(){
		double bestScore = 0;
		Peak bestPeak = null;
		int[] sulfenicMultipliers = {0, 100, 2000, 100, 10, 1, 1, 1, 1, 1};
		for(Peak peak : sulfenicPeaks){
			int charge = peak.getCharge();
			double intensity = peak.getIntensity();
			double score = intensity * sulfenicMultipliers[charge];
			if(score > bestScore){
				bestScore = score;
				bestPeak = peak;
			}
		}
		int[] thialMultipliers = {0, 1000, 20000, 1000, 100, 1, 1, 1, 1, 1};
		for(Peak peak : thialPeaks){
			int charge = peak.getCharge();
			double intensity = peak.getIntensity();
			double score = intensity * thialMultipliers[charge];
			if(score > bestScore){
				bestScore = score;
				bestPeak = peak;
			}
		}
		return bestPeak;
	}
}
