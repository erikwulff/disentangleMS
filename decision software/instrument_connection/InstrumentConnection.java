package instrument_connection;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;

import resources.Timer;
import data_structures.Peak;
import data_structures.Spectrum;

public class InstrumentConnection {
	String dataFile = (new File(getClass().getProtectionDomain().getCodeSource().
			getLocation().getPath())).getParentFile().getPath() + "\\data.txt";
	String commandFile = (new File(getClass().getProtectionDomain().getCodeSource().
			getLocation().getPath())).getParentFile().getPath() + "\\commands.txt";
	StringBuilder queuedOutput = new StringBuilder();
	int position = 0;
	int forwardPosition = 0;
	int count = 0;
	Timer readTimer =  new Timer();

	public Spectrum fromInstrument(){
		if(readTimer.ms() < 10)
			return null;
		readTimer.reset();

		Spectrum spectrum = new Spectrum();
		try{
			FileReader fr = new FileReader(dataFile);
			BufferedReader br = new BufferedReader(fr);
			br.skip(position);
			String line;
			if((line = br.readLine()) != null){
				forwardPosition += line.length() + 2;
				String[] headerTokens = line.split(" ");
				if(headerTokens.length == 7){
					spectrum.scanNumber = Integer.parseInt(headerTokens[0]);
					spectrum.IT = Double.parseDouble(headerTokens[1]);
					spectrum.ST = Double.parseDouble(headerTokens[2]);
					spectrum.analyzer = Integer.parseInt(headerTokens[4]);
					spectrum.parentMass = Double.parseDouble(headerTokens[5]);
					spectrum.scanMode = Integer.parseInt(headerTokens[6]);
					while((line = br.readLine()) != null){
						forwardPosition += line.length() + 2;
						if(line.equals("---end of data---")){
							position = forwardPosition;
							count--;
							br.close();
							fr.close();
							return spectrum;
						}
						String[] dataTokens = line.split(" ");
						if(dataTokens.length == 2){
							double mz = Double.parseDouble(dataTokens[0]);
							double intensity = Double.parseDouble(dataTokens[1]);
							spectrum.add(new Peak(mz, intensity));
						}
					}
				}
			}
			forwardPosition = position;
			br.close();
			fr.close();
		} catch(Exception e){}
		return null;
	}
	
	public void toInstrument(Spectrum spectrum){
		Spectrum s = spectrum;
		setScanMode(s.scanMode);
		setScanRange(s.scanRangeStart, s.scanRangeEnd);
		setAnalyzer(s.analyzer);
		if(s.analyzer == 1)
			setResolution(s.resolution);
		while(s.getParentPeak() != null){
			if(s == spectrum)
				setReactionType(s.scanMode - 1, s.reactionType);
			else
				setReactionType(s.scanMode - 1, 0);
			setParentMZ(s.scanMode - 1, s.parentMass);
			setIsolationWidth(s.scanMode - 1, s.isolationWidth);
			if(s.reactionType == 0){
				setActivationEnergy(s.scanMode - 1, s.activationEnergy);
				setActivationQ(s.scanMode - 1, s.activationQ);
				setActivationTime(s.scanMode - 1, s.activationTime);	
			}
			s = s.getParentSpectrum();
		}
		takeScan();
	}
	
	public void setScanRange(double rangeStart, double rangeEnd){
		queuedOutput.append("setScanRange:" + rangeStart + ":" + rangeEnd);
		queuedOutput.append(System.lineSeparator());
	}
	
	public void setScanMode(int scanMode){
		queuedOutput.append("setScanMode:" + scanMode + ":" + 0);
		queuedOutput.append(System.lineSeparator());
	}
	
	public void setParentMZ(int level, double parentMZ){
		queuedOutput.append("setParentMZ:" + level + ":" + parentMZ);
		queuedOutput.append(System.lineSeparator());
	}
	
	public void setIsolationWidth(int level, double isolationWidth){
		queuedOutput.append("setIsolationWidth:" + level + ":" + isolationWidth);
		queuedOutput.append(System.lineSeparator());
	}
	
	public void setActivationEnergy(int level, double activationEnergy){
		queuedOutput.append("setActivationEnergy:" + level + ":" + activationEnergy);
		queuedOutput.append(System.lineSeparator());
	}
	
	public void setActivationQ(int level, double activationQ){
		queuedOutput.append("setActivationQ:" + level + ":" + activationQ);
		queuedOutput.append(System.lineSeparator());
	}
	
	public void setActivationTime(int level, double activationTime){
		queuedOutput.append("setActivationTime:" + level + ":" + activationTime);
		queuedOutput.append(System.lineSeparator());
	}
	
	public void takeScan(){
//		System.out.println(count);
		queuedOutput.append("takeScan:" + 0 + ":" + 0);
		queuedOutput.append(System.lineSeparator());
		try{
			FileWriter fw = new FileWriter(commandFile, true);
	//		System.out.println(System.getProperty("user.dir") + "\\commands.txt");
		//	System.out.println("C:\\Users\\Bjorn-Erik\\workspace\\ddMassSpec\\bin\\commands.txt");
			queuedOutput.append("---end of commands---");
			queuedOutput.append(System.lineSeparator());
		    fw.write(queuedOutput.toString());
		    fw.close();
			count++;
		}catch (Exception e) {}
		queuedOutput.setLength(0);
	}
	
	public void setAnalyzer(int analyzer){
		queuedOutput.append("setAnalyzer:" + analyzer + ":" + 0);
		queuedOutput.append(System.lineSeparator());
	}
	
	public void setResolution(int resolution){
		queuedOutput.append("setResolution:" + resolution + ":" + 0);
		queuedOutput.append(System.lineSeparator());
	}
	
	public void setReactionType(int level, int type){
		queuedOutput.append("setReactionType:" + level + ":" + type);
		queuedOutput.append(System.lineSeparator());
	}
	
	
	

}
