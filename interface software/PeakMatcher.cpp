
#include "StdAfx.h"
#include "PeakMatcher.h"
#include "PeakSet.h"
#include "Peak.h"

using namespace std;


PeakMatcher::PeakMatcher(double intensityWeight, double intensityAccuracy, double mzWeight, double mzAccuracy){
	this->intensityWeight	= intensityWeight;
	this->intensityAccuracy = intensityAccuracy;
	this->mzWeight			= mzWeight;
	this->mzAccuracy		= mzAccuracy;
}

double PeakMatcher::compare(Peak* peak1, Peak* peak2){
	if( peak1 == NULL || peak2 == NULL || peak1->getIntensity() == 0 || peak2->getIntensity() == 0 )
		return 100000000000000.0;
	double mzScore = pow( abs(peak1->getMZ() - peak2->getMZ()) / mzAccuracy, 2 );
	double intensityRatio = peak1->getIntensity() / peak2->getIntensity();
	double intensityScore = abs( log(intensityRatio) / log(intensityAccuracy) );
	return mzWeight * mzScore + intensityWeight * intensityScore;
}

Peak* PeakMatcher::findPeakMatching(PeakSet* searchSet, Peak* targetPeak){
	if( searchSet == NULL || targetPeak == NULL )
		return NULL;
	Peak* bestMatch = NULL;
	double bestScore = 100000000;
	double lowerBound = targetPeak->getMZ() - 2 * mzAccuracy;
	double upperBound = targetPeak->getMZ() + 2 * mzAccuracy;
	int index = searchSet->getIndexLeftOf( upperBound );
	while( index >= 0 && searchSet->getPeak(index)->getMZ() > lowerBound ){
		Peak* match = searchSet->getPeak(index);
		double score = compare(match, targetPeak);
		if(score < bestScore){
			bestMatch = match;
			bestScore = score;
		}
		index--;
	}
	return bestMatch;
}

PeakSet* PeakMatcher::findPeaksMatching(PeakSet* searchSet, PeakSet* targetSet, scaling mode){
	if( searchSet == NULL || targetSet == NULL )
		return NULL;
	if( mode == WITHOUT_SCALING )
		return findPeaksMatchingWithoutScaling(searchSet, targetSet);
	return findPeaksMatchingAllowingScaling(searchSet, targetSet);
}

PeakSet* PeakMatcher::findPeaksUniquelyMatching(PeakSet* searchSet, PeakSet* targetSet, scaling mode){
	if( searchSet == NULL || targetSet == NULL )
		return NULL;
	if( mode == WITHOUT_SCALING )
		return findPeaksUniquelyMatchingWithoutScaling(searchSet, targetSet);
	return findPeaksUniquelyMatchingAllowingScaling(searchSet, targetSet);
}

PeakSet* PeakMatcher::findPeaksMatchingWithoutScaling(PeakSet* searchSet, PeakSet* targetSet){
	PeakSet* matches = new PeakSet();
	for(int n = 0; n < targetSet->size(); n++)
		matches->add( findPeakMatching(searchSet, targetSet->getPeak(n) ) );
	return matches;
}

PeakSet* PeakMatcher::findPeaksMatchingAllowingScaling(PeakSet* searchSet, PeakSet* targetSet){
	return NULL;
}

PeakSet* PeakMatcher::findPeaksUniquelyMatchingAllowingScaling(PeakSet* searchSet, PeakSet* targetSet){
	return NULL;
}

PeakSet* PeakMatcher::findPeaksUniquelyMatchingWithoutScaling(PeakSet* searchSet, PeakSet* targetSet){
	return NULL;
}