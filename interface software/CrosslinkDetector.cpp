
#include "StdAfx.h"

#include "Spectrum.h"
#include "Peak.h"
#include <math.h>
#include "IsotopeVariants.h"
using namespace std;
#include <vector>
#include <algorithm>
#include "CrosslinkDetector.h"

bool CrosslinkDetector::findCrosslink( Spectrum* spectrum ){
	findBestAnnotation( spectrum );
	if( isCrosslink( spectrum ) ){
		dhaHit = choseDhaPeak();
		thialHit = choseThialPeak();
		return true;
	}
	return false;
}

//Determines what parent charge and dehydroalanine mass would best explain the fragments seen
 void CrosslinkDetector::findBestAnnotation( Spectrum* spectrum ){
	Peak* parentPeak = spectrum->getParentPeak();
	double parentMZ = parentPeak->getMZ();
	int parentCharge = parentPeak->getCharge();
	double parentMass = parentMZ * parentCharge;

	double bestScore = -1;
	for( int i = 0; i < spectrum->getNumberOfVariants(); i++){
		Variants* dehydroalaninePeaks = spectrum->getVariants(i);
		double dhaMass = dehydroalaninePeaks->getDeisotopedDechargedMass();
		double sulfenicMass = parentMass - dhaMass;
		double thialMass = sulfenicMass - WATER;
		Variants* sulfenicPeaks = spectrum->getVariantsNearest(sulfenicMass);
		Variants* thialPeaks = spectrum->getVariantsNearest(thialMass);
		if(sulfenicPeaks == NULL || thialPeaks == NULL)
			continue;

		double score = 0;
		for( int q = 1; q < parentCharge; q++){
			IsotopeVariants* dhas = dehydroalaninePeaks->getIsotopologuesWithCharge(q);
			IsotopeVariants* sulfenics = sulfenicPeaks->getIsotopologuesWithCharge(parentCharge - q);
			IsotopeVariants* thials = thialPeaks->getIsotopologuesWithCharge(parentCharge - q);

			if(dhas->size() == 0 || sulfenics->size() == 0 || thials->size() == 0)
				continue;
			if( dhas->getFirstPeak()->getMZ() > parentMZ - 20 && dhas->getFirstPeak()->getMZ() < parentMZ + 5 )
				continue;
			if( sulfenics->getFirstPeak()->getMZ() > parentMZ - 20 && sulfenics->getFirstPeak()->getMZ() < parentMZ + 5 )
				continue;
			if( thials->getFirstPeak()->getMZ() > parentMZ - 20 && thials->getFirstPeak()->getMZ() < parentMZ + 5 )
				continue;

			score += dhas->getTotalIntensity();
			score += sulfenics->getTotalIntensity();
			score += thials->getTotalIntensity();
		}

		if( score > bestScore ){
			bestScore = score;
			this->dehydroalaninePeaks = dehydroalaninePeaks;
			this->sulfenicPeaks = sulfenicPeaks;
			this->thialPeaks = thialPeaks;
		}
	}
}

//Determine whether MS3s should be done
bool CrosslinkDetector::isCrosslink( Spectrum* spectrum ){
	double parentCharge = spectrum->getParentPeak()->getCharge();
	double parentMZ = spectrum->getParentPeak()->getMZ();
	double waterLossMZ = parentMZ - 18.0 / parentCharge;
	PeakSet* parentPeaks = spectrum->getPeaksInRadius(parentMZ, 4.0);
	PeakSet* waterLossPeaks = spectrum->getPeaksInRadius(waterLossMZ, 4.0);

	// Sum up various kinds of intensities
	double intensityTotal = spectrum->getTotalIntensity();
	double intensityMatch = dehydroalaninePeaks->getTotalIntensity() + 
							thialPeaks->getTotalIntensity() + sulfenicPeaks->getTotalIntensity();
	double intensityCommon = parentPeaks->getTotalIntensity() + waterLossPeaks->getTotalIntensity();
	double intensityOther = intensityTotal - intensityMatch - intensityCommon;

	delete parentPeaks, waterLossPeaks;

	// Check whether intensities from cross-links are large enough
	if( intensityOther != 0 && intensityMatch / intensityOther < 1 / 2 )
		return false;
	if( ( intensityOther + intensityCommon ) != 0 && intensityMatch / ( intensityOther + intensityCommon ) < 1 / 6)
		return false;

	//Require at least one dehydroalanine peak that has both matching sulfenic and thial peaks
	for(int q = 0; q < parentCharge; q++){
		double dhaIntensity		= dehydroalaninePeaks->getIsotopologuesWithCharge(q)->getTotalIntensity();
		double sulfenicIntensity	= sulfenicPeaks->getIsotopologuesWithCharge(parentCharge - q)->getTotalIntensity();
		double thialIntensity		= thialPeaks->getIsotopologuesWithCharge(parentCharge - q)->getTotalIntensity();
		if( dhaIntensity > intensityTotal / 100 )
			if( sulfenicIntensity > dhaIntensity / 20 )
				if( thialIntensity > sulfenicIntensity / 25 )
					return true;
	}

	return false;
}

Peak* CrosslinkDetector::choseDhaPeak(){
	Peak* bestDhaPeak = NULL;
	int multipliers[] = {100, 2000, 100, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	double bestScore = 0;
	for( int d = 0; d < dehydroalaninePeaks->size(); d++ ){
		Peak* peak = dehydroalaninePeaks->getPeak(d);
		int charge = peak->getCharge();
		double intensity = peak->getIntensity();
		double score = intensity * multipliers[charge];
		if( score > bestScore ){
			bestDhaPeak = peak;
			bestScore = score;
		}
	}
	return 	bestDhaPeak;
}

Peak* CrosslinkDetector::choseThialPeak(){
	Peak* bestThialPeak = NULL;
	int sulfenicMultipliers[] = {100, 2000, 100, 10, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	double bestScore = 0;
	for( int s = 0; s < sulfenicPeaks->size(); s++ ){
		Peak* peak = sulfenicPeaks->getPeak(s);
		int charge = peak->getCharge();
		double intensity = peak->getIntensity();
		double score = intensity * sulfenicMultipliers[charge];
		if( score > bestScore ){
			bestThialPeak = peak;
			bestScore = score;
		}
	}
	int thialMultipliers[] = {1000, 20000, 1000, 100, 10, 10, 10, 10, 10, 10, 10, 10, 10};
	for( int t = 0; t < sulfenicPeaks->size(); t++ ){
		Peak* peak = thialPeaks->getPeak(t);
		int charge = peak->getCharge();
		double intensity = peak->getIntensity();
		double score = intensity * thialMultipliers[charge];
		if( score > bestScore ){
			bestThialPeak = peak;
			bestScore = score;
		}
	}
	return bestThialPeak;
}

Peak* CrosslinkDetector::get_dhaPeak(){
	return dhaHit;
}

Peak* CrosslinkDetector::get_thialPeak(){
	return thialHit;
}