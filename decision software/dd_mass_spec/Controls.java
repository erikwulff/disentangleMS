package dd_mass_spec;
import instrument_connection.ScanManager;
import analysis.Deconvolutor;
import data_structures.Peak;
import data_structures.Spectrum;


public class Controls {
	private ScanManager scanManager = new ScanManager();
	private Deconvolutor deconvolutor = new Deconvolutor();

	public void MS1(){
		Spectrum spectrum = new Spectrum();
		spectrum.setScanParameters(400, 2000, 0, 0.0, 0.0, 0.0, 0.0, 0.0);
		spectrum.analyzer = 1;
		spectrum.resolution = 60000;
		spectrum.reactionType = 0;
		spectrum.isolationWidth = 2.0;
		scanManager.enqueue(spectrum);
	}
	
	public void highEnergyFragment(Peak peak){
		fragment(peak, 35.0, 0.250, 10.0);
	}
	
	public void lowEnergyFragment(Peak peak){
		double CIDEnergy = 8 + ((int) (10 * peak.getMZ() / 120)) / 10.0;   // round to one decimal
		fragment(peak, CIDEnergy, 0.170, 30.0);
	}
	
	public void fragment(Peak peak, double CID, double ActQ, double time){
		Spectrum spectrum = new Spectrum();
		int scanMode = peak.getSpectrum().scanMode + 1;
		int minMZ = Math.max((int)peak.getMZ() / 4, 150);
		int maxMZ = Math.min((int)peak.getMZ() * peak.getCharge(), 2000);
		spectrum.setScanParameters(minMZ, maxMZ, scanMode, peak.getMZ(), 1.0, CID, ActQ, time);
		spectrum.analyzer = 0;
		spectrum.resolution = 4000;
		spectrum.reactionType = 0;
		spectrum.isolationWidth = 2.0;
		peak.setFragments(spectrum);
		scanManager.enqueue(spectrum);
	}
	
	public Spectrum getData(){
		Spectrum spectrum = scanManager.getData();
		deconvolutor.deconvolute(spectrum);
		return spectrum;
	}

	public boolean hasData(){
		return scanManager.hasData();
	}
	
	public int numberOnIT(){
		return scanManager.numberOnIT();
	}
	
	public int numberOnFT(){
		return scanManager.numberOnFT();
	}
	
	public int numberOnITAndFT(){
		return numberOnIT() + numberOnFT();
	}
	
}
