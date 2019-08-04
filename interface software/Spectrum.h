#pragma once

#include <string>
#include <vector>
#include "PeakSet.h"
#include "Peak.h"
#include "Variants.h"
#include "IsotopeVariants.h"
#include "ChargeVariants.h"

using namespace std;

class Spectrum : public PeakSet {
	vector<Variants*> bretheren;
	vector<IsotopeVariants*> isotopologues;
	vector<ChargeVariants*> chargemers;
	vector<Variants*> allBretheren;
	Peak* parentPeak;
	void add(Peak* peak){}
	void add( PeakSet* set ){}
	void remove(Peak* peak){}

public:
	string annotation;
	double requestTime;

	//Scan Parameter Data
	double scanRangeStart, scanRangeEnd;
	int scanMode;
	double parentMass, isolationWidth;
	double activationEnergy, activationQ, activationTime;
	int analyzer, reactionType;
	ULONG resolution;

	//Scan Data
	int scanNumber;
	float IT, ST;

	Spectrum();

	// Deletes all peaks in the spectrum. This recursively deletes all child spectra and variants.
	~Spectrum();

	// Sets the scan parameters.
	void setScanParameters( double scanRangeStart, double scanRangeEnd,   int scanMode, 
						    double parentMass,     double isolationWidth, double activationEnergy, 
						    double activationQ,    double activationTime );

	// Sets the scan data.
	void setScanData( int scanNumber,         float IT,    float ST,
					  unsigned numberOfPeaks, double* mzs, double* intensities );

	// Adds a new peak to the spectrum with given mz and intensity.
	// If a peak with mz of <mz> is already in the spectrum, does nothing.
	bool addPeak( double mz, double intensity );

	// Returns the parent peak's spectrum.
	Spectrum* getParentSpectrum();

	// Returns the parent peak.
	Peak* getParentPeak();

	// Sets spectrum's annotation to <str>.
	void annotate( string str );

	// Appends <str> to previous current annotation.
	void appendToAnnotation( string str );

	// Returns spectrum's annotation.
	string* getAnnotation();

	Variants* newVariants();

	// Adds <variants> to the spectrum, 
	// so it to be returned when the spectrum is querried for variants.
	void add(Variants* variants);

	// Removes <variants >from the spectrum,
	// so it can not be returned when the spectrum is querried for variants.
	// Spectrum still keeps tracks of <variants>,
	// so it is automatically deleted when the spectrum is deleted.
	void remove(Variants* variants);

	// Adds <isotopeVariants> to the spectrum,
	// so it to be returned when the spectrum is querried for isotope variants.
	void add(IsotopeVariants* isotopeVariants);

	// Removes <isotopeVariants >from the spectrum,
	// so it can not be returned when the spectrum is querried for isotope variants.
	// Spectrum still keeps tracks of <isotopeVariants>,
	// so it is automatically deleted when the spectrum is deleted.
	void remove(IsotopeVariants* isotopeVariants);

	// Adds <chargeVariants> to the spectrum,
	// so it to be returned when the spectrum is querried for charge variants.
	void add(ChargeVariants* chargeVariants);

	// Removes <chargeVariants >from the spectrum,
	// so it can not be returned when the spectrum is querried for charge variants.
	// Spectrum still keeps tracks of <chargeVariants>,
	// so it is automatically deleted when the spectrum is deleted.
	void remove(ChargeVariants* chargeVariants);
	
	// Returns set of peaks that represent same molecule
	// with deisotoped and decharged mass nearest <mass>
	Variants* getVariantsNearest( double mass );

	// Returns set of peaks that represent same molecule
	// with deisotoped mz nearest <mass>
	IsotopeVariants* getIsotopeVariantsNearest( double mz );

	// Returns set of peaks that represent same molecule
	// with decharged mass nearest <mass>
	ChargeVariants* getChargeVariantsNearest( double mass );

	// Returns number of variants this spectrum can return.
	int getNumberOfVariants(){ return bretheren.size(); }

	// Returns number of isotope variants this spectrum can return.
	int getNumberOfIsotopeVariants(){ return isotopologues.size(); }
	
	// Returns number of charge variants this spectrum can return.
	int getNumberOfChargeVariants(){ return chargemers.size(); }

	// Returns variants number n+1.
	Variants* getVariants( int n ){ return bretheren.at(n); }

	// Returns isotope variants number n+1.
	IsotopeVariants* getIsotopeVariants( int n ){ return isotopologues.at(n); }

	// Returns charge variants number n+1.
	ChargeVariants* getChargeVariants( int n ){ return chargemers.at(n); }
	
	friend class Peak;
};