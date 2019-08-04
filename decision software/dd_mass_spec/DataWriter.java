package dd_mass_spec;
import java.io.FileWriter;
import java.util.Iterator;

import resources.Timer;
import data_structures.Peak;
import data_structures.Spectrum;

public class DataWriter {
	Timer runTime = new Timer();
	
	
	void add(Spectrum MS1){
		if(MS1 == null)
			return;
		try{
			FileWriter fw = new FileWriter(System.getProperty("user.dir") + "\\output.txt", true);
			//FileWriter fw = new FileWriter("C:\\Users\\Bjorn-Erik\\Desktop\\output.txt", true);
			
			fw.write("t:" + runTime.minutes());
			fw.write(System.lineSeparator());
			fill(MS1, fw);

		    fw.close();
		}catch (Exception e) {}
	}
	
	void fill(Spectrum MS1, FileWriter fw){
		try{
			Iterator<Peak> it = MS1.iterateByMZ();
			while(it.hasNext()){
				Peak peak = it.next();
				
				for(int i = 0; i <= MS1.scanMode; i++)
					fw.write("\t");
				fw.write("m:" + peak.getMZ() + ";");
				fw.write("i:" + peak.getIntensity() + ";");
				fw.write("q:" + Math.max(peak.getCharge(), 1) + ";");
				fw.write("in:" + Math.max(peak.getIsotopeNumber(), 0) + ";");
				
	//			if(peak.isotopeVariants == null)
	//				fw.write("isotopeSerie:" + 0 + ";");
	//			else
	//				fw.write("isotopeSerie:" + peak.isotopeVariants.id + ";");
				
				
				if(peak.getVariants() == null)
					fw.write("bretheren:" + 0 + ";");
				else
					fw.write("bretheren:" + peak.getVariants().id() + ";");
				
	//			if(peak.chargeVariants == null)
	//				fw.write("chargemers:" + 0 + ";");
	//			else
	//				fw.write("chargemers:" + peak.chargeVariants.id + ";");
		
				if(peak.color.equals("grey")){
					if(peak.getFragments() != null){
						fw.write("c:" + "black" + "");
						fw.write(System.lineSeparator());
						fill(peak.getFragments(), fw);
					} else {
						fw.write("c:" + peak.color + "");
						fw.write(System.lineSeparator());
					}			
				} else {
					fw.write("c:" + peak.color + "");
					fw.write(System.lineSeparator());
					fill(peak.getFragments(), fw);
				}
					
			}
			
		}catch (Exception e) {}
	}
	

}
