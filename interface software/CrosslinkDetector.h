#pragma once

#include "Spectrum.h"
#include "Peak.h"
#include "Parameters.h"
#include "PeakSet.h"
#include <vector>

#define WATER 18.0

enum annotations{DEHYDROALANINE, THIAL, SULFENIC};

class CrosslinkDetector{
public:
	bool findCrosslink( Spectrum* fragments );
	Peak* get_dhaPeak();
	Peak* get_thialPeak();

protected:
	Peak* dhaHit;
	Peak* thialHit;

	Variants* dehydroalaninePeaks;
	Variants* sulfenicPeaks;
	Variants* thialPeaks;

	void findBestAnnotation( Spectrum* spectrum );
	bool isCrosslink( Spectrum* spectrum );
	Peak* choseDhaPeak();
	Peak* choseThialPeak();
};
