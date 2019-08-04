package dd_mass_spec;

import java.util.Iterator;

import resources.Timer;
import analysis.CrosslinkDetector;
import data_structures.Peak;
import data_structures.Spectrum;

public class Logic {
	Controls control = new Controls();
	DataWriter writer = new DataWriter();
	CrosslinkDetector crosslinkDetector = new CrosslinkDetector();
	ExclusionList exclusionList = new ExclusionList(10.0, 3.0);
	Spectrum MS1 = null;
	Spectrum oldMS1 = null;
	Timer dutyCycleTimer = new Timer(Parameters.dutyCycleLength);
	Iterator<Peak> iterator;
	
	void run(){
		Timer.resetUniversalTime();
		Timer runTime = new Timer();
		
		while(runTime.seconds() < Parameters.runTime - 10){
			if(control.numberOnFT() < 3)
				control.MS1();

			if(control.numberOnIT() < 3)
				getAnotherMS2();
			
			if(control.hasData()){
				Spectrum spectrum = control.getData();
				if(spectrum.scanMode == 0)
					setNewMS1(spectrum);
				else if(spectrum.scanMode == 1)
					actOnMS2Data(spectrum);
			}
		}
	}
	
	void getAnotherMS2(){
		if(iterator != null)
			while(iterator.hasNext()){
				Peak peak = iterator.next();
				if(exclusionList.isNotExcluded(peak)){
					control.lowEnergyFragment(peak);
					exclusionList.add(peak);
					break;
				}
			}
	}
	
	public void setNewMS1(Spectrum spectrum){
		writer.add(oldMS1);
		oldMS1 = MS1;
		MS1 = spectrum;
		iterator = spectrum.iterateByIntensity();
		dutyCycleTimer.reset();
	}
	
	public void actOnMS2Data(Spectrum spectrum){
		if(crosslinkDetector.findCrosslink(spectrum)){
			control.highEnergyFragment(crosslinkDetector.getDhaPeak());
			control.highEnergyFragment(crosslinkDetector.getThialPeak());
			System.out.println(" Suspected XL, parent mz: " + spectrum.parentMass);
			System.out.println("\tDonor mz: " + crosslinkDetector.getDhaPeak().getMZ());
			System.out.println("\tAcceptor mz: " + crosslinkDetector.getThialPeak().getMZ());
		}
	}
}
