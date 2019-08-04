package analysis;
import data_structures.Peak;
import data_structures.PeakSet;
import dd_mass_spec.Parameters;


public class ChargeSeriePredictor {
	
	public PeakSet predict(double dechargedMass){
		PeakSet peakSet = new PeakSet();
		for(int q = 1; q <= Parameters.maxCharge; q++){
			double mz = (dechargedMass + q * Constants.H1Mass) / q;
			double intensity = 1000;
			if(mz >= 50 && mz < 2000){
				Peak peak = new Peak(mz, intensity);
				peak.setCharge(q);
				peakSet.add(peak);
			}
		}
		return peakSet;
	}

}
