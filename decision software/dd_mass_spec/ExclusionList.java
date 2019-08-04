package dd_mass_spec;
import java.util.LinkedList;

import resources.Timer;
import data_structures.Peak;


public class ExclusionList {
	LinkedList<Peak> exclusions = new LinkedList<Peak>();
	LinkedList<Double> times = new LinkedList<Double>();
	Timer time = new Timer();
	double exclusionTime;
	double riseFactor;

	ExclusionList(double exclusionTime, double riseFactor){
		this.exclusionTime = exclusionTime;
		this.riseFactor = riseFactor;
	}
	
	public void add(Peak peak){
		if(peak.getVariants() != null)
			for(Peak p : peak.getVariants())
				addToList(p);
		else
			addToList(peak);
	}
	
	private void addToList(Peak peak){
		exclusions.offer(peak);
		times.offer(time.seconds());
	}
	
	public boolean isExcluded(Peak peak){
		clearExpired();
		for(Peak p : exclusions)
			if(Math.abs(peak.getMZ() - p.getMZ()) < 0.05)
				if(peak.getIntensity() < p.getIntensity() * riseFactor )
					return true;
		return false;
	}
	
	public boolean isNotExcluded(Peak peak){
		return !isExcluded(peak);
	}
	
	private void clearExpired(){
		while(!exclusions.isEmpty()){
			if(times.peekFirst() > time.seconds() + exclusionTime){
				times.pollFirst();
				exclusions.pollFirst();
			} else
				break;
		}
	}
}
