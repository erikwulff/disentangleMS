#pragma once

#include "Spectrum.h"
#include "PeakSet.h"
#include "IsotopeVariants.h"
#include "Peak.h"
#include "SeriePredictor.h"

#define NUMBER_OF_CHARGES 7
#define NUMBER_OF_ISOTOPES 8

class Deconvolutor{
	double relativeIntensities[400][NUMBER_OF_ISOTOPES]; //[molecule mass / 50][isotope number]
	int deconvolutionLimit;
	SeriePredictor* seriePredictor;

public:
	Deconvolutor();
	// Deisotopes and deconvolutes <spectrum>.
	void deconvolute(Spectrum* spectrum);

protected:
	void deisotope(Spectrum* spectrum);
	void deisotope(Peak* peak, PeakSet* s, Spectrum* spectrum);
	void dechargeDeisotopedSeries(Spectrum* spectrum);
	void decharge(Spectrum* spectrum);
	
	double binomial_coefficient(int n, int k);
	vector<IsotopeVariants*>* getPossibleIsotopeSeries(Peak* peak);

};