package data_structures;

import java.util.Iterator;
import java.util.TreeSet;

import resources.Timer;

public class Spectrum extends PeakSet {
	public TreeSet<Variants> variantsList = new TreeSet<Variants>(new VariantComparator());
	public double scanRangeStart, scanRangeEnd;
	public int scanMode;
	public double parentMass, isolationWidth;
	public double activationEnergy, activationQ, activationTime;
	public int analyzer, reactionType;
	public int resolution;
	public int scanNumber;
	public double IT, ST;
	public long requestTime;
	
	private Peak parentPeak;
	
	public Spectrum(){
		requestTime = Timer.universalMS();
	}
	
	public void setScanParameters(double scanRangeStart, double scanRangeEnd, int scanMode, 
			double parentMass, double isolationWidth, double activationEnergy,
			double activationQ, double activationTime){
		this.scanRangeStart = scanRangeStart;
		this.scanRangeEnd = scanRangeEnd;
		this.scanMode = scanMode;
		this.parentMass = parentMass;
		this.isolationWidth = isolationWidth;
		this.activationEnergy = activationEnergy;
		this.activationQ = activationQ;
		this.activationTime = activationTime;
	}
	
	public void setScanData(int scanNumber, double IT, double ST, int numberOfPeaks, double[] mzs, double[] intensities){
		this.scanNumber = scanNumber;
		this.IT = IT;
		this.ST = ST;
		for(int i = 0; i < numberOfPeaks; i++)
			add(new Peak(mzs[i], intensities[i]));
	}

	public void setScanData(int scanNumber, double IT, double ST, int numberOfPeaks, PeakSet peakSet){
		this.scanNumber = scanNumber;
		this.IT = IT;
		this.ST = ST;
		add(peakSet);
		for(Peak peak : peakSet)
			peak.setSpectrum(this);
	}
	
	protected void setParentPeak(Peak parentPeak){
		this.parentPeak = parentPeak;
	}
	
	public Spectrum getParentSpectrum(){
		if(parentPeak == null)
			return null;
		else
			return parentPeak.getSpectrum();
	}
	
	public Peak getParentPeak(){
		return parentPeak;
	}
	
	public void addVariants(Variants variants){
		if(variants != null)
			variantsList.add(variants);
	}
	
	public void removeVariants(Variants variants){
		variantsList.remove(variants);
	}
	
	public Iterator<Variants> iterateVariants(){
		return variantsList.iterator();
	}
	
	public Variants getVariantsNearest(double baseMass){
		double nearestSeparation = 3 * baseMass / resolution;
		Variants nearestVariants = null;
		for(Variants variants : variantsList){
			double separation = Math.abs(variants.getBaseMass() - baseMass);
			if(separation < nearestSeparation){
				nearestSeparation = separation;
				nearestVariants = variants;
			}
			if(variants.size() == 0)
				break;
		}
		return nearestVariants;
	}
}
