#pragma once

#include <string>
#include "PeakSet.h"
#include "Peak.h"
#include "Spectrum.h"

using namespace std;

// Can only be created and deleted by it's parent variants.
// Will be deleted when, and only when, it's parent variants is deleted.
class ChargeVariants : public PeakSet {
	Spectrum* spectrum;
	int isotopeNumber;

	ChargeVariants(int isotopeNumber, Spectrum* spectrum);
	~ChargeVariants(){}
	void remove(Peak* peak);
	void add(Peak* peak);

public:
	int getIsotopeNumber(){ return isotopeNumber; }
	double getDechargedMass();

	friend class Variants;
};