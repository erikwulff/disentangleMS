package data_structures;
import java.util.Iterator;
import java.util.TreeMap;


public class Variants extends PeakSet implements Comparable<Variants>{
	Spectrum spectrum = null;

	public Variants(Spectrum spectrum){
		this.spectrum = spectrum;
	}
	
	public PeakSet add(PeakSet peakSet){
		if(peakSet != null)
			for(Peak peak : peakSet)
				add(peak);
		return this;
	}

	public Variants add(Peak peak){
		if(peak != null){
			super.add(peak);
			peak.setVariants(this);
			if(size() == 1)
				spectrum.addVariants(this);
		}
		return this;
	}

	public Variants remove(Peak peak){
		super.remove(peak);
		if(size() == 0)
			spectrum.removeVariants(this);
		return this;
	}
	
	public double getBaseMass(){
		double numerator = 0;
		double denominator = 0;
		for(Peak peak : this){
			numerator += peak.getBaseMass();
			denominator++;
		}
		return numerator / denominator;
	}
	
	public void mergeWith(Variants otherVariants){
		if(this.id == otherVariants.id)
			return;
		PeakSet copy = otherVariants.copy();
		for(Peak peak : copy){
			add(peak);
		}
	}
	
	public boolean noChargeOverlap(Variants otherVariants){
		TreeMap<Integer, PeakSet> thisOne = getPeaksByCharge();
		TreeMap<Integer, PeakSet> otherOne = otherVariants.getPeaksByCharge();
		for(int q = 1; q <= 10; q++)
			if(thisOne.get(q) != null && otherOne.get(q) != null)
				return false;
		return true;
	}
	
	public void println(){
		Iterator<Peak> it = iterateByMZ();
		System.out.format("Variants   size = %4d   base mass = %9.3f%n", size(), getBaseMass());
		while(it.hasNext())
			it.next().println();
	}
	
	public int compareTo(Variants otherVariants) {
		if(id() > otherVariants.id())
			return 1;
		else if(id() < otherVariants.id())
			return -1;
		else
			return 0;
	}
}
