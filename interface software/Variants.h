#pragma once

#include <string>
#include "PeakSet.h"
#include "Peak.h"
#include "IsotopeVariants.h"
#include "ChargeVariants.h"

using namespace std;

// Can only be created and deleted by parent spectrum.
// Will be deleted when, and only when, it's parent spectrum is deleted.
class Variants : public PeakSet {
	vector<IsotopeVariants*> isotopeVariants;
	vector<ChargeVariants*> chargeVariants;
	Spectrum* spectrum;
	
	Variants(Spectrum* spectrum);
	~Variants();

public:
	void add(PeakSet* peakSet);
	void add(Peak* peak);
	void remove(PeakSet* peakSet);
	void remove(Peak* peak);

	IsotopeVariants* getIsotopologuesWithCharge(int charge);
	ChargeVariants* getChargemersWithIsotopeNumber(int isotopeNumber);
	void changeIsotopeNumber(Peak* peak, int isotopeNumber);
	void changeChargeState(Peak* peak, int chargeState);
	double getDeisotopedDechargedMass();

	friend class Spectrum;
};