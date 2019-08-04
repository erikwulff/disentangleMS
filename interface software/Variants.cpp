
#include "StdAfx.h"
#include <algorithm>
#include <math.h>
#include "Spectrum.h"
#include "Variants.h"

Variants::Variants(Spectrum* spectrum){
	this->spectrum = spectrum;
	for(int i = 0; i < 10; i++){
		isotopeVariants.push_back( new IsotopeVariants(i, spectrum ) );
		chargeVariants.push_back( new ChargeVariants(i, spectrum ) );
	}
}

Variants::~Variants(){
	for(int i = 0; i < 10; i++){
		delete isotopeVariants.at(i);
		delete chargeVariants.at(i);
	}
}

// adds the peaks in peakset and sets up the isotope and charge variants.
void Variants::add(PeakSet* peakSet){
	if( peakSet != NULL )
		for( int i = 0; i < peakSet->size(); i++ )
			add( peakSet->getPeak(i) );
}

void Variants::add(Peak* peak){
	// don't do anything if peak is null or is not part of the same spectrum as this variants
	if( peak == NULL || peak->getSpectrum() != spectrum )
		return;
	// don't do anything if peak is already among variants
	if( find(peaks[BY_MZ].begin(), peaks[BY_MZ].end(), peak) != peaks[BY_MZ].end() )
		return;
	PeakSet::add(peak);
	getIsotopologuesWithCharge( peak->getCharge() )->add(peak);
	getChargemersWithIsotopeNumber( peak->getIsotopeNumber() )->add(peak);
	peak->setVariants( this );
	if( size() == 1 )
		spectrum->add(this);
}

void Variants::remove(PeakSet* peakSet){
	if( peakSet != NULL )
		for( int i = 0; i < peakSet->size(); i++ )
			remove( peakSet->getPeak(i) );
}

void Variants::remove(Peak* peak){
	// don't do anything if peak is null or is already not among isotope charge variants
	if( peak == NULL || find(peaks[BY_MZ].begin(), peaks[BY_MZ].end(), peak) == peaks[BY_MZ].end() )
		return;
	PeakSet::remove(peak);
	getIsotopologuesWithCharge( peak->getCharge() )->remove(peak);
	getChargemersWithIsotopeNumber( peak->getIsotopeNumber() )->remove(peak);
	peak->setVariants( NULL );
	if( size() == 0 )
		spectrum->remove(this);
}

IsotopeVariants* Variants::getIsotopologuesWithCharge(int charge){
	return isotopeVariants.at(charge);
}

ChargeVariants* Variants::getChargemersWithIsotopeNumber(int isotopeNumber){
	return chargeVariants.at(isotopeNumber);
}

void Variants::changeIsotopeNumber(Peak* peak, int isotopeNumber){
	peak->setIsotopeNumber(isotopeNumber);
	getChargemersWithIsotopeNumber( peak->getIsotopeNumber() )->remove(peak);
	getChargemersWithIsotopeNumber( isotopeNumber )->add(peak);
}

void Variants::changeChargeState(Peak* peak, int charge){
	peak->setCharge(charge);
	getIsotopologuesWithCharge( peak->getCharge() )->remove(peak);
	getIsotopologuesWithCharge( charge )->add(peak);
}

double Variants::getDeisotopedDechargedMass(){
	double sum = 0;
	double normalization = 0;
	for( int i = 0; i < size(); i++ ){
		sum += getPeak(i)->getMZ() * getPeak(i)->getCharge() - getPeak(i)->getIsotopeNumber() - getPeak(i)->getCharge();
		normalization += 1;
	}
	return sum / normalization;
}