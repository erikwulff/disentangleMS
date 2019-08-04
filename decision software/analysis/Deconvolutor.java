package analysis;

import java.util.LinkedList;
import java.util.TreeSet;

import resources.Timer;
import data_structures.MatchSet;
import data_structures.Peak;
import data_structures.PeakSet;
import data_structures.Spectrum;
import data_structures.Variants;
import dd_mass_spec.Parameters;

public class Deconvolutor {
	SeriePredictor seriePredictor = new SeriePredictor(Constants.C13Probability, Constants.carbonFrequency);
	ChargeSeriePredictor chargeSeriePredictor = new ChargeSeriePredictor();
	LinkedList<PeakSet> cache = new LinkedList<PeakSet>();
	Matcher matcher = new Matcher(5000);
	
	// try deconvoluting one section of the spectrum at a time, allowing backtracking
	// then jump to other parts of the spectrum based chargemers
	
	public void deconvolute(Spectrum spectrum){
//		Timer time = new Timer();
		if(spectrum == null)
			return;
		matcher.setResolution(spectrum.resolution);
		
		PeakSet unassigned = spectrum.copy();
		
		if(spectrum.scanMode == 0)
			applyCache(spectrum, unassigned);
		deisotope(spectrum, unassigned);
		dechargeDeisotoped(spectrum);
		decharge(spectrum, unassigned);
//		System.out.println("Took " + time.ms() + " ms for an MS" + (spectrum.scanMode + 1));
	}
	
	void applyCache(Spectrum spectrum, PeakSet unassigned){
//		System.out.print(unassigned.size() + "\t\t");
		LinkedList<PeakSet> toRemove = new LinkedList<PeakSet>();
		for(PeakSet theoreticalVariants : cache){
			MatchSet matchSet = matcher.match(theoreticalVariants, unassigned);
			if(matchSet.score() < 5.0){
				matcher.copyAnnotation(matchSet);
				unassigned.remove(matchSet.set2());
			} else
				toRemove.offer(theoreticalVariants);
		}
		for(PeakSet expiredVariant : toRemove)
			cache.remove(expiredVariant);
//		System.out.println(unassigned.size());
	}
	
	void cache(PeakSet theoreticalVariants){
		cache.offer(theoreticalVariants);
	}
	
	void deisotope(Spectrum spectrum, PeakSet unassigned){
		Timer timer = new Timer();
		while(unassigned.size() > 0 && timer.ms() < Parameters.deconvolutionLimit){
			Peak peak = unassigned.getFirstPeakByIntensity();
			TreeSet<MatchSet> possibleNewMatchSets = new TreeSet<MatchSet>();
			for(int q = 1; q <= Parameters.maxCharge; q++)
				for(int i = 0; i <= 4; i++){
					PeakSet theoretical = seriePredictor.predict(peak, q, i);
					if(theoretical != null && theoretical.size() > 1)
						possibleNewMatchSets.add(matcher.match(theoretical, unassigned));
				}
			possibleNewMatchSets.add(new MatchSet(matcher.match((Peak) null, peak)));
			MatchSet bestMatchSet = possibleNewMatchSets.first();
			matcher.copyAnnotation(bestMatchSet);
			unassigned.remove(bestMatchSet.set2());
			if(spectrum.scanMode == 0)
				cache(bestMatchSet.set1());
		}
//		System.out.print("Deisotope time: " + timer.ms());
//		System.out.print(" Total peaks: " + spectrum.size());
//		System.out.println(" Unassigned peaks: " + unassigned.size());
	}
	
	void recursiveDeisotope(Spectrum spectrum, PeakSet unassigned){
		Timer timer = new Timer();
		while(unassigned.size() > 0 && timer.ms() < Parameters.deconvolutionLimit){
			Peak peak = unassigned.getFirstPeakByIntensity();
			PeakSet mustBeMatched = spectrum.getPeaksInRadius(peak.getMZ(), 5).getPeaksAbove(100.0); 
			PeakSet mayBeMatched = spectrum.getPeaksInRadius(peak.getMZ(), 10);
			MatchSet matches = recursiveDeisotope(new MatchSet(), mustBeMatched, mayBeMatched);
			
			matcher.copyAnnotation(matches);
			unassigned.remove(matches.set2());
			if(spectrum.scanMode == 0)
				cache(matches.set1());	
		}
//		System.out.print("Deisotope time: " + timer.ms());
//		System.out.print(" Total peaks: " + spectrum.size());
//		System.out.println(" Unassigned peaks: " + unassigned.size());
	}
	
	MatchSet recursiveDeisotope(MatchSet matches, PeakSet mustBeMatched, PeakSet mayBeMatched){
		// Base case
		if(mustBeMatched.size() == 0)
			return matches;

		// Enumerate all possible matches
		TreeSet<MatchSet> possibleNewMatchSets = new TreeSet<MatchSet>();
		Peak peak = mustBeMatched.getFirstPeakByIntensity();
		for(int q = 1; q <= Parameters.maxCharge; q++)
			for(int i = 0; i <= 4; i++){
				PeakSet theoretical = seriePredictor.predict(peak, q, i);
				if(theoretical != null && theoretical.size() > 1)
					possibleNewMatchSets.add(matcher.match(theoretical, mayBeMatched));
			}
		possibleNewMatchSets.add(new MatchSet(matcher.match((Peak) null, peak)));

		// Identify the matches worth following up on
		double bestScore = possibleNewMatchSets.first().score();
		LinkedList <MatchSet> promisingNewMatchSets = new LinkedList<MatchSet>();
		for(MatchSet possibleNewMatchSet : possibleNewMatchSets)
			if(possibleNewMatchSet.score() < bestScore + 0.4)
				promisingNewMatchSets.add(possibleNewMatchSet);
		
		// Recurse on identified matches
		TreeSet<MatchSet> results = new TreeSet<MatchSet>();
		for(MatchSet promisingNewMatchSet : promisingNewMatchSets){
			MatchSet newMatchSet = matches.copy().add(promisingNewMatchSet);
			PeakSet newMustBeMatched = mustBeMatched.copy().remove(promisingNewMatchSet.set2());
			PeakSet newMayBeMatched = mayBeMatched.copy().remove(promisingNewMatchSet.set2());
			results.add(recursiveDeisotope(newMatchSet, newMustBeMatched, newMayBeMatched));
		}
		
		// Return best result from all the recursions
		return results.first();
	}
	
	void lowResolutionInference(Spectrum spectrum){
		// if resolution is low
		// look at charge 1 and charge 2
		// see if those can inform other chargemers
		// if so, change those chargemers
	}
	
	// problem with modifying what's being iterated?
	void dechargeDeisotoped(Spectrum spectrum){
		double tolerance = 2000.0 / spectrum.resolution;
		@SuppressWarnings("unchecked")
		TreeSet<Variants> clone = (TreeSet<Variants>) spectrum.variantsList.clone();
		for(Variants variants1 : clone)
			for(Variants variants2 : clone)
				if(Math.abs(variants1.getBaseMass() - variants2.getBaseMass()) < tolerance)
					if(variants1.noChargeOverlap(variants2))
						variants1.mergeWith(variants2);
	}
	
	void decharge(Spectrum spectrum, PeakSet unassigned){
		Timer timer = new Timer();
		while(unassigned.size() > 0 && timer.ms() < Parameters.deconvolutionLimit){
			Peak peak = unassigned.getFirstPeakByIntensity();
			MatchSet bestMatchSet = null;
			for(int q = 1; q <= Parameters.maxCharge; q++){
				double dechargedMass = peak.getMZ() * q - q * Constants.H1Mass;
				PeakSet theoretical = chargeSeriePredictor.predict(dechargedMass);
				if(theoretical.size() > 1){
					MatchSet matchSet = matcher.matchLargest(theoretical, unassigned);
					if(matchSet.set2().size() >= 2)
						if(bestMatchSet == null || matchSet.score() < bestMatchSet.score())
							bestMatchSet = matchSet;
				}
			}
			if(bestMatchSet != null){
				matcher.copyAnnotation(bestMatchSet);
				unassigned.remove(bestMatchSet.set2());
			} else
				unassigned.remove(peak);
		}
	}
}