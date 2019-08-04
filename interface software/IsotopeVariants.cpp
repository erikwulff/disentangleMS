
#include "StdAfx.h"
#include <algorithm>
#include <math.h>
#include "IsotopeVariants.h"
#include "Peak.h"


IsotopeVariants::IsotopeVariants(int charge, Spectrum* spectrum){
	this->charge = charge;
	this->spectrum = spectrum;
}

// Adds <peak> to the set. If peak is NULL or has an mz already in the set, does nothing.
void IsotopeVariants::add( Peak* peak ){
	if( peak == NULL )
		return;
	PeakSet::add(peak);
	if( size() == 1 && peak->getSpectrum() != NULL )
		spectrum->add(this);
}

void IsotopeVariants::remove( Peak* peak ){
	PeakSet::remove(peak);
	if(size() == 0)
		spectrum->remove(this);
}

double IsotopeVariants::getDeisotopedMZ(){
	double sum = 0;
	double normalization = 0;
	for( int i = 0; i < size(); i++ ){
		sum += getPeak(i)->getMZ() - getPeak(i)->getIsotopeNumber() / getPeak(i)->getCharge();
		normalization += 1;
	}
	return sum / normalization;
}

double IsotopeVariants::getDeisotopedDechargedMass(){
	double sum = 0;
	double normalization = 0;
	for( int i = 0; i < size(); i++ ){
		sum += getPeak(i)->getMZ() * getPeak(i)->getCharge() - getPeak(i)->getIsotopeNumber() - getPeak(i)->getCharge();
		normalization += 1;
	}
	return sum / normalization;
}