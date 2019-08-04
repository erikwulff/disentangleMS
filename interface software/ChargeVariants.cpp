
#include "StdAfx.h"
#include <algorithm>
#include <math.h>
#include "ChargeVariants.h"
#include "Spectrum.h"


// Constructor.
ChargeVariants::ChargeVariants(int isotopeNumber, Spectrum* spectrum){
	this->isotopeNumber = isotopeNumber;
	this->spectrum = spectrum;
}

void ChargeVariants::add( Peak* peak ){
	if( peak == NULL )
		return;
	PeakSet::add(peak);
	if( size() == 1 && peak->getSpectrum() != NULL )
		spectrum->add(this);
}

void ChargeVariants::remove( Peak* peak ){
	PeakSet::remove(peak);
	if(size() == 0)
		spectrum->remove(this);
}


double ChargeVariants::getDechargedMass(){
	double sum = 0;
	double normalization = 0;
	for( int i = 0; i < size(); i++ ){
		sum += getPeak(i)->getMZ() * getPeak(i)->getCharge();
		normalization += 1;
	}
	return sum / normalization;
}