#pragma once

#include <string>
#include "PeakSet.h"
#include "Peak.h"
#include "Spectrum.h"

using namespace std;

// Can only be created and deleted by it's parent variants.
// Will be deleted when, and only when, it's parent variants is deleted.
class IsotopeVariants : public PeakSet {
	Spectrum* spectrum;
	int charge;

	IsotopeVariants(int charge, Spectrum* spectrum);
	~IsotopeVariants(){}
	void add(Peak* peak);
	void remove(Peak* peak);

public:
	int getCharge(){ return charge; }
	double getDeisotopedMZ();
	double getDeisotopedDechargedMass();

	friend class Variants;
};