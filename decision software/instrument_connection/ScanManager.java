package instrument_connection;

import java.util.LinkedList;

import resources.Timer;
import data_structures.Spectrum;

public class ScanManager {
	private LinkedList<Spectrum> onInstrument = new LinkedList<Spectrum>();
	private LinkedList<Spectrum> fromInstrument = new LinkedList<Spectrum>();
	private InstrumentConnection connection = new InstrumentConnection();
	private int numberOnIT = 0;
	private int numberOnFT = 0;
	
	public void enqueue(Spectrum spectrum){
		connection.toInstrument(spectrum);
		onInstrument.add(spectrum);	
		if(spectrum.analyzer == 0)
			numberOnIT++;
		else if(spectrum.analyzer == 1)
			numberOnFT++;
		update();
	}
	
	public Spectrum getMatchingSpectrum(Spectrum data){
		Spectrum spectrum = null;
		for(Spectrum s : onInstrument)
			if(s.scanMode == data.scanMode)
				if(s.analyzer == data.analyzer)
					if(s.scanMode == 0 || Math.abs(s.parentMass - data.parentMass) < 0.01){
						spectrum = s;
						break;
					}
		if(spectrum != null)
			onInstrument.remove(spectrum);
		return spectrum;
	}
	
	public void update(){
		if(!onInstrument.isEmpty()){
			Spectrum data = connection.fromInstrument();
			if(data != null){
				Spectrum spectrum = getMatchingSpectrum(data);
				if(spectrum != null){
					spectrum.setScanData(data.scanNumber, data.IT, data.ST, 0, data);
					fromInstrument.add(spectrum);
					if(spectrum.analyzer == 0)
						numberOnIT--;
					else if(spectrum.analyzer == 1)
						numberOnFT--;
				}
			}
		}
		LinkedList<Spectrum> droppedScans = new LinkedList<Spectrum>();
		for(Spectrum spectrum : onInstrument)
			if(spectrum.requestTime + 20000 < Timer.universalMS())
				droppedScans.add(spectrum);
		for(Spectrum spectrum : droppedScans){
			onInstrument.remove(spectrum);
			if(spectrum.analyzer == 0)
				numberOnIT--;
			if(spectrum.analyzer == 1)
				numberOnFT--;
			System.out.print("Dropped scan: MS" + (spectrum.scanMode + 1));
			if(spectrum.scanMode > 0)
				System.out.print(" resulting from fragmentation of m/z " + spectrum.parentMass);
			System.out.println(".");
		}
	}
	
	public Spectrum getData(){
		update();
		if(fromInstrument.size() > 0)
			return fromInstrument.remove();
		else
			return null;
	}
	
	public boolean hasData(){
		update();
		return fromInstrument.size() > 0;
	}
	
	public int numberOnIT(){
		return numberOnIT;
	}
	
	public int numberOnFT(){
		return numberOnFT;
	}
}
