
#include "StdAfx.h"
#include <math.h>
#include <string>
#include "Parameters.h"
#include "Spectrum.h"
#include "Peak.h"
#include "Variants.h"
#include "IsotopeVariants.h"
#include "ChargeVariants.h"

Peak::Peak( double mz, double intensity, Spectrum* spectrum ){
	this->mz						= mz;
	this->intensity					= intensity;
	this->spectrum					= spectrum;
	this->fragments					= NULL;
	this->annotation				= "";
	this->charge					= 1;
	this->isotopeNumber				= 0;
	this->variants					= NULL;
}

// Sets <spectrum> as child of this peak and this peak as parent of <spectrum>.
void Peak::setFragments( Spectrum* spectrum ){
	if( spectrum != NULL ){
		this->fragments = spectrum;
		spectrum->parentPeak = this;
	}
}

// Returns number indicating how much this peak's values differ from <mz> and <intensity>.
// Exact match will return 0.
double Peak::compareTo( double mz, double intensity ){
	double mzScore = pow( differenceFrom(mz) / Parameters::accuracy, 2 );
	double intensityScore = abs( log( max(10, intensity) / max(10, this->intensity) ) / log(10.0) );
	return mzScore + intensityScore;
}

// Returns number that goes up with how much peak's mz and intensity differ from those of <peak>.
// Exact match will return 0.
double Peak::compareTo( Peak* peak ){
	if( peak == NULL )
		return compareTo( getMZ(), 0 );
	return compareTo( peak->getMZ(), peak->getIntensity() );
}

// Sets charge to <charge>. Makes necessary updates to isotope and charge variants.
void Peak::setCharge(int charge){
	if( this->charge != charge ){
		this->charge = charge;
		if( variants != NULL )
			variants->changeChargeState(this, charge);
	}
}

// Sets isotope number to <isotopeNumber>. Makes necessary updates to isotope and charge variants.
void Peak::setIsotopeNumber(int isotopeNumber){
	if( this->isotopeNumber != isotopeNumber ){
		this->isotopeNumber = isotopeNumber;
		if( variants != NULL )
			variants->changeIsotopeNumber(this, isotopeNumber);
	}
}

// Returns set of peaks in same isotope serie as this one.
// The set will contain at least this peak.
IsotopeVariants* Peak::getIsotopeSerie(){
	if(variants == NULL)
		return NULL;
	return variants->getIsotopologuesWithCharge(charge);
}

// Returns set of peaks that are same molecule and isotopologue as this one,
// but that have a different charge.
// The set will contain at least this peak.
ChargeVariants* Peak::getChargeSerie(){
	if(variants == NULL)
		return NULL;
	return (variants->getChargemersWithIsotopeNumber(isotopeNumber));
}

// Returns set of peaks in same isotope serie as this one.
// The set will contain at least this peak.
Variants* Peak::getVariants(){
	return variants;
}

// Set's the peak's set of isotope and charge variants to <serie>. Updates both the old and the new serie.
// If setting to <serie> would not change the variants, does nothing to prevent infinite recursion.
// Problem is that this should also change isotope series.
void Peak::setVariants(Variants* serie){
	if( serie == NULL || serie == variants )
		return;
	if( variants != NULL )
		variants->remove(this);
	serie->add(this);
	variants = serie;
}