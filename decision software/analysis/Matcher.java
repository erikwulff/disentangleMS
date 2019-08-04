package analysis;

import java.util.TreeMap;

import data_structures.Match;
import data_structures.MatchSet;
import data_structures.Peak;
import data_structures.PeakSet;
import data_structures.Variants;

public class Matcher {
	private int resolution;
	
	public Matcher(int resolution){
		this.resolution = resolution;
	}
	
	public void setResolution(int resolution){
		this.resolution = resolution;
	}
	
	// make use of surrounding noise?
	// takes too long? replace just this one with something that just returns a number?
	public Match match(Peak peak1, Peak peak2){
		double mzScore = 0;
		double intensityScore = 0;
		if(peak1 != null && peak2 != null){
			mzScore = Math.pow(Math.abs(peak1.getMZ() - peak2.getMZ()) / (peak1.getMZ() / (2 * resolution)), 2);
			intensityScore = Math.abs(Math.log(Math.max(1, peak1.getIntensity()) / Math.max(1, peak2.getIntensity())));
		} else if(peak1 != null)
			intensityScore = Math.abs(Math.log(Math.max(1, peak1.getIntensity()) / 1));
		else if(peak2 != null)
			intensityScore = Math.abs(Math.log(1 / Math.max(1, peak2.getIntensity())));
		double score = mzScore + intensityScore;
		if(score < 0.000000000000001)
			score = 0;
		return new Match(peak1, peak2, score);
	}
	
	public Match match(Peak needle, PeakSet haystack){
		PeakSet smallerHaystack = haystack.getPeaksInRadius(needle.getMZ(), 2 * needle.getMZ() / resolution);
		Match bestMatch = match(needle, (Peak) null);
		for(Peak straw : smallerHaystack){
			Match match = match(needle, straw);
			if(bestMatch == null || match.score() < bestMatch.score())
				bestMatch = match;
		}
		return bestMatch;
	}
	
	public MatchSet match(PeakSet needles, PeakSet haystack){
		MatchSet matchSet = new MatchSet();
		if(needles == null || haystack == null)
			return matchSet;
		for(Peak needle : needles)
			matchSet.add(match(needle, haystack));
		return matchSet;
	}
	
	public MatchSet matchLargest(PeakSet needles, PeakSet haystack){
		MatchSet matchSet = new MatchSet();
		for(Peak needle : needles)
			matchSet.add(match(needle, haystack.getLargestPeakInRadius(needle.getMZ(), needle.getMZ() / resolution)));
		return matchSet;	
	}
	
	public void copyAnnotation(MatchSet matchSet){
		TreeMap<Integer, Variants> variants = new TreeMap<Integer, Variants>();
		for(Match match : matchSet){
			Peak annotation = match.peak1();
			if(annotation != null && annotation.getVariants() != null){
				int id = annotation.getVariants().id();
				if(!variants.containsKey(id))
					if(matchSet.set2().isNotEmpty())
						variants.put(id, new Variants(matchSet.set2().getFirstPeakByMZ().getSpectrum()));
			}
		}
		
		for(Match match : matchSet){
			Peak peak = match.peak2();
			if(peak != null){
				Peak annotation = match.peak1();
				if(annotation != null){
					peak.setCharge(annotation.getCharge());
					peak.setIsotopeNumber(annotation.getIsotopeNumber());
					if(annotation.getVariants() != null){
						int id = annotation.getVariants().id();
						variants.get(id).add(peak);
					}
				}
			}
		}
	}
	
	public double getProminence(Peak peak){
		return Math.log10(peak.getIntensity() / peak.getSpectrum().getPeaksInRadius(peak.getMZ(), 0.15).getTotalIntensity());
	}

}
