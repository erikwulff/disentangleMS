package data_structures;


import java.util.Comparator;

public class VariantComparator implements Comparator<Variants> {
	public int compare(Variants variants1, Variants variants2){
		if(variants1.id() > variants2.id())
			return 1;
		else if(variants1.id() < variants2.id())
			return -1;
		else
			return 0;
	}
}
