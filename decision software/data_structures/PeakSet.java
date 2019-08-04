package data_structures;


import java.util.Iterator;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;

public class PeakSet implements Iterable<Peak> {
	private TreeSet<Peak> peaksByMZ = new TreeSet<Peak>(new MZComparator());
	private TreeSet<Peak> peaksByIntensity = new TreeSet<Peak>(new IntensityComparator());
	private double totalIntensity = 0;
	private static int counter = 1;
	public int id = 0;
	
	public PeakSet(){
		id = counter++;
	}
	
	public PeakSet(SortedSet<Peak> peaks){
		id = counter++;
		peaksByMZ.addAll(peaks);
		peaksByIntensity.addAll(peaks);
	}
	
	public Iterator<Peak> iterateByMZ(){
		return peaksByMZ.iterator();
	}
	
	public Iterator<Peak> iterateByIntensity(){
		return peaksByIntensity.iterator();
	}
	
	public PeakSet add(Peak peak){
		if(peak != null && !contains(peak)){
			totalIntensity += peak.getIntensity();
			peaksByMZ.add(peak);
			peaksByIntensity.add(peak);
		}
		return this;
	}
	
	public PeakSet add(PeakSet peakSet){
		if(peakSet != null)
			for(Peak peak : peakSet)
				add(peak);
		return this;
	}
	
	public PeakSet remove(Peak peak){
		if(peak != null && contains(peak)){
			totalIntensity -= peak.getIntensity();
			peaksByMZ.remove(peak);
			peaksByIntensity.remove(peak);
		}
		return this;
	}
	
	public PeakSet remove(PeakSet peakSet){
		if(peakSet != null)
			for(Peak peak : peakSet)
				remove(peak);
		return this;
	}
	
	public boolean contains(Peak peak){
		return peaksByMZ.contains(peak);
	}
	
	public int size(){
		return peaksByMZ.size();
	}
	
	public boolean isEmpty(){
		return peaksByMZ.isEmpty();
	}
	
	public boolean isNotEmpty(){
		return !peaksByMZ.isEmpty();
	}
	
	public Peak getFirstPeakByMZ(){
		if(isEmpty())
			return null;
		else
			return peaksByMZ.first();
	}
	
	public Peak getFirstPeakByIntensity(){
		if(isEmpty())
			return null;
		else
			return peaksByIntensity.first();
	}
	
	public Peak getLastPeakByMZ(){
		if(isEmpty())
			return null;
		else
			return peaksByMZ.last();
	}
	
	public Peak getLastPeakByIntensity(){
		if(isEmpty())
			return null;
		else
			return peaksByIntensity.last();
	}
	
	public Peak getPeakLeftOf(double mz){
		return peaksByMZ.ceiling(new Peak(mz, 1));
	}
	
	public Peak getPeakRightOf(double mz){
		return peaksByMZ.floor(new Peak(mz, 1));
	}
	
	public Peak getPeakMoreIntenseThan(double mz){
		return peaksByIntensity.ceiling(new Peak(mz, 1));
	}
	
	public Peak getPeakLessIntenseThan(double mz){
		return peaksByIntensity.floor(new Peak(mz, 1));
	}
	
	public Peak getPeakLeftOf(Peak peak){
		return peaksByMZ.ceiling(peak);
	}
	
	public Peak getPeakRightOf(Peak peak){
		return peaksByMZ.floor(peak);
	}
	
	public Peak getPeakMoreIntenseThan(Peak peak){
		return peaksByIntensity.ceiling(peak);
	}
	
	public Peak getPeakLessIntenseThan(Peak peak){
		return peaksByIntensity.floor(peak);
	}
	
	public PeakSet getPeaksAbove(double intensity){
		PeakSet copy = new PeakSet();
		for(Peak peak : this)
			if(peak.getIntensity() > intensity)
				copy.add(peak);
		return copy;
	}
	
	public PeakSet getPeaksUnder(double intensity){
		PeakSet copy = new PeakSet();
		for(Peak peak : this)
			if(peak.getIntensity() < intensity)
				copy.add(peak);
		return copy;
	}
	
	public Peak getPeakNearest(double mz){
		Peak peakOnLeft = getPeakLeftOf(mz);
		Peak peakOnRight = getPeakRightOf(mz);
		if(peakOnLeft == null)
			return peakOnRight;
		if(peakOnRight == null)
			return peakOnLeft;
		if(mz - peakOnLeft.getMZ() < peakOnRight.getMZ() - mz)
			return peakOnLeft;
		else
			return peakOnRight;
	}
	
	public Peak getLargestPeakInRange(double lowMZ, double highMZ){
			return getPeaksInRange(lowMZ, highMZ).getFirstPeakByIntensity();
	}
	
	public Peak getLargestPeakInRadius(double centerMZ, double radius){
		return getLargestPeakInRange(centerMZ - radius, centerMZ + radius);
	}
	
	public PeakSet getPeaksInRange(double lowMZ, double highMZ){
		return new PeakSet(peaksByMZ.subSet(new Peak(lowMZ, 1), new Peak(highMZ, 1)));
	}
	
	public PeakSet getPeaksInRadius(double centerMZ, double radius){
		return getPeaksInRange(centerMZ - radius, centerMZ + radius);
	}
	
	public double getTotalIntensity(){
		return totalIntensity;
	}

	public PeakSet copy(){
		PeakSet copy = new PeakSet();
		copy.add(this);
		return copy;
	}

	public Iterator<Peak> iterator() {
		return iterateByIntensity();
	}
	
	public void println(){
		System.out.format("PeakSet   size = %4d%n", size());
		for(Peak peak : peaksByMZ)
			peak.println();
	}
	
	public PeakSet getPeaksWithIsotopeNumber(int isotopeNumber){
		PeakSet result = new PeakSet();
		for(Peak peak : this)
			if(peak.getIsotopeNumber() == isotopeNumber)
				result.add(peak);
		return result;
	}
	
	public PeakSet getPeaksWithCharge(int charge){
		PeakSet result = new PeakSet();
		for(Peak peak : this)
			if(peak.getCharge() == charge)
				result.add(peak);
		return result;
	}
	
	public TreeMap<Integer, PeakSet> getPeaksByIsotopeNumber(){
		TreeMap<Integer, PeakSet> result = new TreeMap<Integer, PeakSet>();
		for(Peak peak : this){
			PeakSet set = result.get(peak.getIsotopeNumber());
			if(set == null){
				set = new PeakSet();
				result.put(peak.getIsotopeNumber(), set);
			}
			set.add(peak);
		}
		return result;
	}
	
	public TreeMap<Integer, PeakSet> getPeaksByCharge(){
		TreeMap<Integer, PeakSet> result = new TreeMap<Integer, PeakSet>();
		for(Peak peak : this){
			PeakSet set = result.get(peak.getCharge());
			if(set == null){
				set = new PeakSet();
				result.put(peak.getCharge(), set);
			}
			set.add(peak);
		}
		return result;
	}
	
	public int id(){
		return id;
	}
	
}
