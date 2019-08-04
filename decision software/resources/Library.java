package resources;

public class Library {

	public static double binomial_coefficient(int n, int k){
		if(k == 0 || n == k)
			return 1;
		if(k > n)
			return 0;
		if(k > (n - k))
			k = n - k;
		double b = 1;
		for(int i = 1; i <= k; i++){
			b *= (n - (k - 1));
			b /= i;
		}
		return b;
	}

	public static double binomial(double probability, int n, int k){
		return Math.pow(probability, k) * Math.pow(1 - probability, n - k) * Library.binomial_coefficient(n, k);
	}
	
}
