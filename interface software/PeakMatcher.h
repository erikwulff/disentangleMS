#pragma once

#include "PeakSet.h"
#include "Peak.h"

enum scaling{ WITHOUT_SCALING, ALLOWING_SCALING, NUMBER_OF_SCALING_SCHEMES };


class PeakMatcher{
	double intensityWeight;
	double intensityAccuracy;
	double mzWeight;
	double mzAccuracy;

public:
	double compare(Peak* peak1, Peak* peak2);
	PeakMatcher(double intensityWeight, double intensityAccuracy, double mzWeight, double mzAccuracy);
	Peak* findPeakMatching(PeakSet* searchSet, Peak* targetPeak);
	PeakSet* findPeaksMatching(PeakSet* searchSet, PeakSet* targetSet, scaling mode = WITHOUT_SCALING);
	PeakSet* findPeaksUniquelyMatching(PeakSet* searchSet, PeakSet* targetSet, scaling mode = WITHOUT_SCALING);

private:
	PeakSet* findPeaksMatchingAllowingScaling(PeakSet* searchSet, PeakSet* targetSet);
	PeakSet* findPeaksMatchingWithoutScaling(PeakSet* searchSet, PeakSet* targetSet);
	PeakSet* findPeaksUniquelyMatchingAllowingScaling(PeakSet* searchSet, PeakSet* targetSet);
	PeakSet* findPeaksUniquelyMatchingWithoutScaling(PeakSet* searchSet, PeakSet* targetSet);
};