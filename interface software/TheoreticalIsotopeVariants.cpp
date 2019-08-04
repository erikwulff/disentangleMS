
#include "StdAfx.h"
#include "TheoreticalIsotopeVariants.h"
#include "Peak.h"

// Constructor.
TheoreticalIsotopeVariants::TheoreticalIsotopeVariants(){
	charge = 1;
	inferredBaseMass = 1000.0;
}

TheoreticalIsotopeVariants::TheoreticalIsotopeVariants(PeakSet* peakSet){
	add(peakSet);
	charge = 1;
	inferredBaseMass = 1000.0;
}

TheoreticalIsotopeVariants::~TheoreticalIsotopeVariants(){
	for( int i = 0; i < size(); i++ )
		delete getPeak(i);
}

// Adds all peaks in <set> to the set. If <set> is NULL, does nothing.
void TheoreticalIsotopeVariants::add( PeakSet* peakSet ){
	if( peakSet != NULL )
		for( int i = 0; i < peakSet->size(); i++ )
			add( peakSet->getPeak(i) );
}

// Adds <peak> to the set. If peak is NULL or has an mz already in the set, does nothing.
void TheoreticalIsotopeVariants::add( Peak* peak ){
	if( peak != NULL )
		PeakSet::add(peak);
}

void TheoreticalIsotopeVariants::remove( Peak* peak ){
	PeakSet::remove(peak);
}