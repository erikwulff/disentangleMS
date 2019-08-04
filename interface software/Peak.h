#pragma once

#include <string>
class Spectrum;
class Variants;
class IsotopeVariants;
class ChargeVariants;
enum orders{ BY_MZ, BY_INTENSITY, NUMBER_OF_ORDERING_SCHEMES };

using namespace std;

// Can only be created by it's parent spectrum.
// Will be deleted when, and only when, it's parent spectrum is deleted.
class Peak{
	double mz;
	double intensity;
	int charge;
	int isotopeNumber;
	string annotation;
	Spectrum* spectrum;
	Spectrum* fragments;
	Variants* variants;

	Peak( double mz, double intensity, Spectrum* spectrum );
	~Peak(){ delete fragments; }

public:
	// Returns the spectrum that this peak is part of.
	Spectrum* getSpectrum(){ return spectrum; }

	// Returns the child spectrum of this peak.
	// If this peak has no child spectrum (has never been fragmented), returns NULL.
	Spectrum* getFragments(){ return fragments; }

	// Sets <spectrum> as child of this peak and this peak as parent of <spectrum>.
	void setFragments( Spectrum* spectrum );

	// Returns peak's mz.
	double getMZ(){ return mz; }

	// Returns peak's intensity.
	double getIntensity(){ return intensity; }

	// Returns the difference between this peak's mz and <mz>.
	double differenceFrom( double mz ){	return abs( this->mz - mz ); }

	// Returns number that goes up with how much peak's values differ from <mz> and <intensity>.
	// Exact match will return 0.
	double compareTo( double mz, double intensity );

	// Returns number that goes up with how much peak's mz and intensity differ from those of <peak>.
	// Exact match will return 0.
	double compareTo( Peak* peak );

	// Sets peak's annotation to <str>.
	void annotate( string str ){ annotation = str; }

	// Appends <str> to previous current annotation.
	void appendToAnnotation( string str ){ annotation.append(str); }

	// Returns peak's annotation.
	string* getAnnotation(){ return &annotation; }

	// Returns peak's charge state annotation.
	int getCharge(){ return charge; }

	// Sets peak's charge state annotation to <charge>.
	void setCharge(int charge);
	
	// Returns peak's isotope number annotation.
	// I.e., if peak contains no heavy isotopes, returns 0.
	// If it has one heavy isotope, returns 1. And so on.
	int getIsotopeNumber(){ return isotopeNumber; }

	// Sets peak's isotope number annotation.
	void setIsotopeNumber(int charge);

	// Returns set of peaks that are isotopologues of this one.
	// The set will contain at least this peak.
	IsotopeVariants* getIsotopeSerie();

	// Returns set of peaks that are same molecule and isotopologue as this one,
	// but that have a different charge.
	// The set will contain at least this peak.
	ChargeVariants* getChargeSerie();

	// Returns set of peaks that are same molecule as this one,
	// but have different charge and/or isotope number.
	// The set will contain at least this peak.
	Variants* getVariants();

	// Annotates this peak as being a charge or isotope variant of the molecule
	// represented by <serie>.
	void setVariants(Variants* serie);

	friend class Spectrum;
};
