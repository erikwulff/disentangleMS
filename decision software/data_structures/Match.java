package data_structures;

public class Match {
	private Peak peak1 = null;
	private Peak peak2 = null;
	private double score = -1;
	private Spectrum spectrum1 = null;
	private Spectrum spectrum2 = null;
	
	public Match(Peak peak1, Peak peak2, double score){
		this.peak1 = peak1;
		this.peak2 = peak2;
		if(peak1 != null)
			spectrum1 = peak1.getSpectrum();
		if(peak2 != null)
			spectrum2 = peak2.getSpectrum();
		this.score = score;
	}
	
	public Match(Peak peak1, Peak peak2){
		this(peak1, peak2, -1);
		if(peak1 != null)
			spectrum1 = peak1.getSpectrum();
		if(peak2 != null)
			spectrum2 = peak2.getSpectrum();
	}
	
	public Peak peak1(){
		return peak1;
	}
	
	public Peak peak2(){
		return peak2;
	}
	
	public Spectrum spectrum1(){
		return spectrum1;
	}
	
	public Spectrum spectrum2(){
		return spectrum2;
	}

	public double score(){
		return score;
	}
	
	public void print(){
		if(peak1 != null)
			peak1.print();
		else
			System.out.print("(                        )");
		System.out.print(" - ");
		if(peak2 != null)
			peak2.print();
		else
			System.out.print("(                        )");
		if(score == 0)
			System.out.format(" %8.2f", score);
		else if(score < 0.000001 || score > 999)
			System.out.format(" %5.2e", score);
		else
			System.out.format(" %8.2f", score);
	}
	
	public void println(){
		print();
		System.out.println();
	}
	//flip method
}
