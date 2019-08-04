
#include "StdAfx.h"
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include "Spectrum.h"
#include "Variants.h"
#include "IsotopeVariants.h"
#include "ChargeVariants.h"

bool intensityComparator( Peak* peak1, Peak* peak2 ){ return peak1->getIntensity() > peak2->getIntensity(); }

Spectrum::Spectrum(){
	parentPeak = NULL;
	annotation = "";
}

// Deletes all peaks in the spectrum. This recursively deletes all child spectra.
Spectrum::~Spectrum(){
	if( parentPeak != NULL )
		parentPeak->fragments = NULL;
	for( int i = 0; i < allBretheren.size(); i++)
		delete allBretheren[i];
	for( int i = 0; i < peaks[BY_MZ].size(); i++ )
		delete peaks[BY_MZ][i];
}

// Sets the scan parameters.
void Spectrum::setScanParameters( double scanRangeStart, double scanRangeEnd, int scanMode,
								  double parentMass, double isolationWidth, double activationEnergy, 
								  double activationQ, double activationTime ){
	this->scanRangeStart	= scanRangeStart;
	this->scanRangeEnd		= scanRangeEnd;
	this->scanMode			= scanMode;
	this->parentMass		= parentMass;
	this->isolationWidth	= isolationWidth;
	this->activationEnergy	= activationEnergy;
	this->activationQ		= activationQ;
	this->activationTime	= activationTime;
}

// Sets the scan data.
void Spectrum::setScanData( int scanNumber, float IT, float ST,
							unsigned numberOfPeaks, double* mzs, double* intensities ){
	this->scanNumber		= scanNumber;
	this->IT				= IT;
	this->ST				= ST;

	for( int i = 0; i < numberOfPeaks; i++ ){
		peaks[BY_MZ].push_back( new Peak( mzs[i], intensities[i], this ) );
		peaks[BY_INTENSITY].push_back( peaks[BY_MZ].at(i) );
		totalIntensity += intensities[i];
	}

	for( int i = 1; i < numberOfPeaks; i++ )
		for( int j = i; peaks[BY_MZ][j]->getMZ() < peaks[BY_MZ][j-1]->getMZ() && j > 0; j-- )
			swap( peaks[BY_MZ][j], peaks[BY_MZ][j-1] );

	sort( peaks[BY_INTENSITY].begin(), peaks[BY_INTENSITY].end(), intensityComparator );
}

// Adds a new peak to the spectrum with given mz and intensity.
// If a peak with mz of <mz> is already in the spectrum, does nothing.
bool Spectrum::addPeak( double mz, double intensity ){
	if( mz >= 100 && mz <= 2000 && intensity > 0 ){
		Peak* peak = new Peak(mz, intensity, this);
		return PeakSet::add(peak);									// should check if there is already a peak with this mz
	}
	return false;
}

// Returns the parent peak's spectrum.
Spectrum* Spectrum::getParentSpectrum(){
	return parentPeak->getSpectrum();
}

// Returns the parent peak.
Peak* Spectrum::getParentPeak(){
	return parentPeak;
}

// Sets peak's annotation to <str>.
void Spectrum::annotate( string str ){
	annotation = str;
}

// Changes the annotation from "x" to "x".<str>.
void Spectrum::appendToAnnotation( string str ){
	annotation.append(str);
}

// Returns peak's annotation.
string* Spectrum::getAnnotation(){
	return &annotation;
}

void Spectrum::add(IsotopeVariants* i){
	if( i != NULL && find(isotopologues.begin(), isotopologues.end(), i) == isotopologues.end() )
		isotopologues.push_back(i);
}

void Spectrum::add(ChargeVariants* icv){
	if( icv != NULL && find(chargemers.begin(), chargemers.end(), icv) == chargemers.end() )
		chargemers.push_back(icv);
}

Variants* Spectrum::newVariants(){
	Variants* v = new Variants(this);
	allBretheren.push_back( v );
	return v;
}

void Spectrum::add(Variants* icv){
	if( icv != NULL && find(bretheren.begin(), bretheren.end(), icv) == bretheren.end() )
		bretheren.push_back(icv);
	if( icv != NULL && find(allBretheren.begin(), allBretheren.end(), icv) == allBretheren.end() )
		allBretheren.push_back(icv);
}

void Spectrum::remove(IsotopeVariants* icv){
	vector<IsotopeVariants*>::iterator it = std::remove(isotopologues.begin(), isotopologues.end(), icv);
	isotopologues.erase(it, isotopologues.end());
}

void Spectrum::remove(ChargeVariants* icv){
	vector<ChargeVariants*>::iterator it = std::remove(chargemers.begin(), chargemers.end(), icv);
	chargemers.erase(it, chargemers.end());
}

void Spectrum::remove(Variants* icv){
	vector<Variants*>::iterator it = std::remove(bretheren.begin(), bretheren.end(), icv);
	bretheren.erase(it, bretheren.end());
}

Variants* Spectrum::getVariantsNearest( double mass ){
	Variants* nearest = NULL;
	double nearestSeparation = 200000;
	for( int i = 0; i < bretheren.size(); i++ ){
		double separation = mass - bretheren.at(i)->getDeisotopedDechargedMass();
		if( separation < nearestSeparation ){
			nearestSeparation = separation;
			nearest = bretheren.at(i);
		}
	}
	return nearest;
}

IsotopeVariants* Spectrum::getIsotopeVariantsNearest( double mz ){
	IsotopeVariants* nearest = NULL;
	double nearestSeparation = 200000;
	for( int i = 0; i < isotopologues.size(); i++ ){
		double separation = mz - isotopologues.at(i)->getDeisotopedMZ();
		if( separation < nearestSeparation ){
			nearestSeparation = separation;
			nearest = isotopologues.at(i);
		}
	}
	return nearest;
}

ChargeVariants* Spectrum::getChargeVariantsNearest( double mass ){
	ChargeVariants* nearest = NULL;
	double nearestSeparation = 200000;
	for( int i = 0; i < chargemers.size(); i++ ){
		double separation = mass - chargemers.at(i)->getDechargedMass();
		if( separation < nearestSeparation ){
			nearestSeparation = separation;
			nearest = chargemers.at(i);
		}
	}
	return nearest;
}