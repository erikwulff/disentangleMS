package data_structures;


public class Peak {
	private double mz, intensity;
	private int charge = -1;
	private int isotopeNumber = -1;
	private Spectrum spectrum, fragments;
	private Variants variants = null;
	public String color = "grey";
	static private int idCounter = 1;
	private int id = 0;

	public Peak(double mz, double intensity){
		id = idCounter++;
		this.mz = mz;
		this.intensity = intensity;
	}
	
	public double getMZ(){
		return mz;
	}
	
	public double getIntensity(){
		return intensity;
	}
	
	public Spectrum getFragments(){
		return fragments;
	}
	
	public void setFragments(Spectrum fragments){
		this.fragments = fragments;
		fragments.setParentPeak(this);
	}
	
	public Spectrum getSpectrum(){
		return spectrum;
	}
	
	protected void setSpectrum(Spectrum spectrum){
		this.spectrum = spectrum;
	}

	public int getCharge(){
		return charge;
	}
	
	public void setCharge(int charge){
		this.charge = charge;
	}
	
	public int getIsotopeNumber(){
		return isotopeNumber;
	}
	
	public void setIsotopeNumber(int isotopeNumber){
		this.isotopeNumber = isotopeNumber;
	}
	
	public Variants getVariants(){
		return variants;
	}
	
	public double getBaseMass(){
		return getMZ() * getCharge() - getCharge() - getIsotopeNumber();
	}
	
	protected void setVariants(Variants variants){
		if(this.variants != null)
			this.variants.remove(this);
		this.variants = variants;
	}
	
	public void print(){
		System.out.format("(%8.3f, %5.2e,%2d,%2d)", mz, intensity, charge, isotopeNumber);
	}
	
	public void println(){
		print();
		System.out.println();
	}
	
	public int id(){
		return id;
	}
}
