package data_structures;

import java.util.ArrayList;
import java.util.Iterator;

public class MatchSet implements Iterable<Match>, Comparable<MatchSet> {
	private ArrayList<Match> matches = new ArrayList<Match>();
	private PeakSet set1 = null;
	private PeakSet set2 = null;
	private static int counter = 1;
	public int id = 0;
	
	public MatchSet(){
		id = counter++;
		set1 = new PeakSet();
		set2 = new PeakSet();
	}
	
	public MatchSet(Match match){
		id = counter++;
		set1 = new PeakSet();
		set2 = new PeakSet();
		this.add(match);
	}
	
	public MatchSet add(Match match){
		if(match != null)
			if(match.peak1() != null || match.peak2() != null){
				matches.add(match);
				set1.add(match.peak1());
				set2.add(match.peak2());
			}
		return this;
	}

	public double score(){
		double numerator = 0.0;
		double denominator = 0.0;
		for(Match match : matches){
			double weight = 0;
			if(match.peak1() != null)
				weight = Math.pow(2, Math.log(Math.max(match.peak1().getIntensity(), 2)) / Math.log(100));
			else if(match.peak2() != null)
				weight = Math.pow(2, Math.log(Math.max(match.peak2().getIntensity(), 2)) / Math.log(100));
			numerator += match.score() * weight;
			denominator += weight;
		}
		return numerator / denominator;
	}
	
	public Iterator<Match> iterator() {
		return matches.iterator();
	}
	
	public PeakSet set1(){
		return set1;
	}
	
	public PeakSet set2(){
		return set2;
	}
	
	public int size(){
		return matches.size();
	}
	
	public int id(){
		return id;
	}
	
	public void println(){
		System.out.format("MatchSet   size = %4d   score = ", size());
		if(score() == 0)
			System.out.format(" %8.2f%n", score());
		else if(score() < 0.000001 || score() > 999)
			System.out.format(" %5.2e%n", score());
		else
			System.out.format(" %8.2f%n", score());
		for(Match match : matches)
			match.println();
	}

	public int compareTo(MatchSet otherMatchSet) {
		if(this.score() > otherMatchSet.score())
			return 1;
		else if(this.score() < otherMatchSet.score())
			return -1;
		if(this.id() > otherMatchSet.id())
			return 1;
		if(this.id() < otherMatchSet.id())
			return -1;
		else
			return 0;
	}
	
	public MatchSet copy(){
		MatchSet copy = new MatchSet();
		copy.add(this);
		return copy;
	}
	
	public MatchSet add(MatchSet matchSet){
		if(matchSet != null)
			for(Match match : matchSet)
				add(match);
		return this;
	}
}
