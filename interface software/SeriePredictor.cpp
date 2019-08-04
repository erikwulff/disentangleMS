
#include "StdAfx.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include "Spectrum.h"
#include "PeakSet.h"
#include <algorithm>
#include "Peak.h"
#include "SeriePredictor.h"
#include "Parameters.h"
#include "Variants.h"
#include "IsotopeVariants.h"
#include "TheoreticalIsotopeVariants.h"
#include <ctime>
using namespace std;

SeriePredictor::SeriePredictor(){
	for(int i = 0; i < NUMBER_OF_CHARGES * 2000 / 50; i++){
		double mass = i * 50;
		int numberOfCarbons = (int)( 0.044 * mass );
		double C13Probability = 0.0107;
		double C12Probability = 1 - C13Probability;

		for( int x = 0; x < NUMBER_OF_ISOTOPES; x++ ){
			double binomialCoefficient = binomial_coefficient(numberOfCarbons, x);
			double unweightedProbability = pow(C13Probability, x) * pow(C12Probability, numberOfCarbons - x);
			relativeIntensities[i][x] = binomialCoefficient * unweightedProbability;
		}
	}
}

// Predicts an isotope envelope of charge <charge> in which <peak> is the <isotopeNumber>'th peak.
// Returns a spectrum containing these isotopes.
// If peak is part of an MS1, assumes shape of of isotope envelope is peptide-like.
// If peak is part of an MSn (n > 1), uses shape of isolated peaks from MS(n-1).
// This requires that the MS(n-1) has been deconvoluted.
// If any parameters don't make sense, returns NULL.
// Caller is responsible for deleting returned spectrum.
Spectrum* SeriePredictor::predict(Peak* peak, int charge, int isotopeNumber){
	if( peak == NULL )
		return NULL;
	double baseMass = peak->getMZ() * charge - isotopeNumber - charge;
	Spectrum* beforeScaling = predict( baseMass, charge, peak->getSpectrum() );

	if( beforeScaling == NULL || beforeScaling->size() == 0 ){
		delete beforeScaling;
		return NULL;
	}
	Peak* equivalentToReferencePeak = NULL;
	for(int i = 0; i < beforeScaling->size(); i++)
		if( beforeScaling->getPeak(i)->getIsotopeNumber() == isotopeNumber )
			equivalentToReferencePeak = beforeScaling->getPeak(i);
	if( equivalentToReferencePeak == NULL || equivalentToReferencePeak->getIntensity() == 0 ){
		delete beforeScaling;
		return NULL;
	}

	double scaleFactor = peak->getIntensity() / equivalentToReferencePeak->getIntensity();
	Spectrum* afterScaling = new Spectrum();
	for( int i = 0; i < beforeScaling->size(); i++ ){
		Peak* peak = beforeScaling->getPeak(i);
		if( afterScaling->addPeak( peak->getMZ(), peak->getIntensity() * scaleFactor ) ){
			afterScaling->getLastPeak(BY_MZ)->setIsotopeNumber( beforeScaling->getPeak(i)->getIsotopeNumber() );
			afterScaling->getLastPeak(BY_MZ)->setCharge( beforeScaling->getPeak(i)->getCharge() );
		}
	}
	delete beforeScaling;
	return afterScaling;
}

// Predicts an isotope envelope of charge <charge> and deisotopes mass <baseMass>.
// Returns a spectrum containing these isotopes.
// If peak is part of an MS1, assumes shape of of isotope envelope is peptide-like.
// If peak is part of an MSn (n > 1), uses shape of isolated peaks from MS(n-1).
// This requires that the MS(n-1) has been deconvoluted.
// If any parameters don't make sense, returns NULL.
// Caller is responsible for deleting returned spectrum.
Spectrum* SeriePredictor::predict(double baseMass, int charge, Spectrum* spectrum){
	if( spectrum == NULL )
		return NULL;
	if( spectrum->scanMode == 0 )
		return MS1(baseMass, charge);
	else
		return MSn(baseMass, charge, spectrum);
}

// Returns a spectrum containing the typical isotope enelope for a peptide
// with deisotoped mass <baseMass> and charge <charge>.
// If parameters are out of range, returns NULL.
// Caller is responsible for deleting returned spectrum.
Spectrum* SeriePredictor::MS1(double baseMass, int charge){
	if( charge < 1 || charge > NUMBER_OF_CHARGES || baseMass < 0 || baseMass > charge * 2000 )
		return NULL;
	int lookUp = (int)(baseMass / 50);
	Spectrum* newSet = new Spectrum();
	for( int i = 0; i < NUMBER_OF_ISOTOPES; i++ )
		if( newSet->addPeak( (baseMass + charge + i) / charge, relativeIntensities[lookUp][i] ) ){
			newSet->getLastPeak(BY_MZ)->setIsotopeNumber(i);
			newSet->getLastPeak(BY_MZ)->setCharge(charge);
		}
	return newSet;
}

// Returns a spectrum containing an isotope envelope with
// deisotoped mass <baseMass> and charge <charge>.
// Shape of envelope is based on isolation of parent peaks to make <spectrum>.
// Parent spectrum must therefore already be deconvoluted.
// If parameters are out of range, returns NULL.
// Caller is responsible for deleting returned spectrum.
Spectrum* SeriePredictor::MSn(double baseMass, int charge, Spectrum* spectrum){
	if( spectrum == NULL )
		return NULL;

	Peak* parentPeak = spectrum->getParentPeak();
	if( parentPeak == NULL )
		return NULL;

	IsotopeVariants* parentVariants = parentPeak->getIsotopeSerie();
	if( parentVariants == NULL )
		return NULL;

	double isolatedMZ = spectrum->parentMass;
	double isolationWidth = spectrum->isolationWidth;
	Spectrum* isolatedPeaks = isolationPredictor.predict( parentVariants, isolatedMZ, isolationWidth );
	if( isolatedPeaks == NULL || isolatedPeaks->getNumberOfIsotopeVariants() == 0 )
		return NULL;

	Spectrum* result = MSn(baseMass, charge, isolatedPeaks->getIsotopeVariants(0));
	delete isolatedPeaks;
	return result;
}

// Returns a spectrum containing an isotope envelope with
// deisotoped mass <baseMass> and charge <charge>.
// Shape of envelope is based on the assumption that the spectrum results
// from fragmentation of peaks in <parentIsotopologues>.
// If parameters are out of range, returns NULL.
// The returned spectrum has the potential to be empty.
// Caller is responsible for deleting returned spectrum.
Spectrum* SeriePredictor::MSn(double baseMass, int charge, IsotopeVariants* parentIsotopologues){
	if( parentIsotopologues == NULL || parentIsotopologues->size() == 0 )
		return NULL;
	if( charge < 1 || charge > parentIsotopologues->getCharge() )
		return NULL;
	if( baseMass < 0 || baseMass > parentIsotopologues->getDeisotopedDechargedMass() )
		return NULL;

	double parentBaseMass = parentIsotopologues->getDeisotopedDechargedMass();
	int highestIsotopeNumber = parentIsotopologues->getLastPeak(BY_MZ)->getIsotopeNumber();

	vector<double> sums(highestIsotopeNumber + 1, 0.0);
	for( int i = 0; i < parentIsotopologues->size(); i++ )								// each isolated parent peak
		for( int j = 0; j <= parentIsotopologues->getPeak(i)->getIsotopeNumber(); j++ )	// each fragment isotopologue
			sums.at(j) += binomial(baseMass / parentBaseMass, i, j) 
							* parentIsotopologues->getPeak(i)->getIntensity() 
							/ parentIsotopologues->getTotalIntensity();

	Spectrum* newSet = new Spectrum();
	for( int i = 0; i <= highestIsotopeNumber; i++)
		if( sums.at(i) > 0 )
			if( newSet->addPeak( (baseMass + charge + i) / charge, sums.at(i) ) ){
				newSet->getLastPeak(BY_MZ)->setIsotopeNumber(i);
				newSet->getLastPeak(BY_MZ)->setCharge(charge);
			}
	return newSet;
}

double SeriePredictor::binomial(double probability, int n, int k){
	double p1 = max(min(probability, 1.0), 0.0);
	double p2 = max(1 - p1, 0.0);
	return pow(p1, k) * pow(p2, n - k) * binomial_coefficient(n, k);
}

double SeriePredictor::binomial_coefficient(int n, int k){
	if( k == 0 || n == k )
		return 1;
	if( k > n )
		return 0;
	if( k > ( n - k ) )
		k = n - k;
	double b = 1;
	for( int i = 1; i <= k; i++ ){
		b *= ( n - ( k - i ) );
		b /= i;
	}
	return b;
}