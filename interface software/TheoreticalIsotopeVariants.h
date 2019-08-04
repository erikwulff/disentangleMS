#pragma once

#include <string>
#include "PeakSet.h"
#include "Peak.h"

using namespace std;

// Is not associated with a spectrum.
// It's peaks should not be part of anything else.
// Will delete its peaks upon own deletion.
class TheoreticalIsotopeVariants : public PeakSet {
public:
	int charge;
	double inferredBaseMass;

	TheoreticalIsotopeVariants();
	TheoreticalIsotopeVariants(PeakSet* peakSet);
	~TheoreticalIsotopeVariants();

	void add( PeakSet* peakSet );
	void add( Peak* peak );
	void remove(Peak* peak);

	friend class Spectrum;
};