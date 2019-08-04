#pragma once

#include "Peak.h"
#include "PeakSet.h"
#include <vector>

class IsolationPredictor{
public:
	Spectrum* predict(Peak* peak);
	Spectrum* predictSingleVariant(Peak* peak);
	Spectrum* predict(PeakSet* set, double isolatedMZ, double isolationWidth);
	double isolationEfficiency(double mz, double isolatedMZ, double isolationWidth);
};
