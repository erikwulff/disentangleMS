package data_structures;

import java.util.Comparator;

public class IntensityComparator implements Comparator<Peak> {
	public int compare(Peak peak1, Peak peak2){
		if(peak1.getIntensity() < peak2.getIntensity())
			return 1;
		else if(peak1.getIntensity() > peak2.getIntensity())
			return -1;
		else if(peak1.id() > peak2.id())
			return 1;
		else if(peak1.id() < peak2.id())
			return -1;
		else
			return 0;
	}
}
