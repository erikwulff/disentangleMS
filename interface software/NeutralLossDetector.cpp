#include "StdAfx.h"
//	for(int i = 0; i < p.crosslinks.size(); i++)
//		checkForCrosslinking(MS2, parentPeak, p.crosslinks[i].mass, p.crosslinks[i].tolerance, 
//								p.crosslinks[i].minCharge, p.crosslinks[i].maxCharge, strdup(p.crosslinks[i].color.c_str())); //string vs char*


/*



*/
/*
vector<Peak*>* detectNeutralLoss(Peak* parent, ScanData* fragments){

	return new vector<Peak*>;
}



/*


void Logic::checkForNeutralMassLoss(ScanData* MS2, Peak* parentPeak, double neutralMass, double tolerance, int minChargeState, int maxChargeState, char* colorCode){
	for(int i = 0; i < MS2->numberOfMasses; i++)
		if(isNeutralMassLoss(parentPeak, MS2->get_peak(i), neutralMass, tolerance, minChargeState, maxChargeState)){
			MS2->get_peak(i)->set_Color(colorCode);
			parentPeak->set_Color(colorCode);
			MS3(MS2->get_peak(i));
		}
}

bool Logic::isNeutralMassLoss(Peak* parentPeak, Peak* childPeak, double neutralMass, double tolerance, int minChargeState, int maxChargeState){
	if(childPeak->intensity < 1000)
		return false;
//	if(childPeak->intensity < (parentPeak->parentSpectrum->totalIntensity - parentPeak->intensity) / 16.0) // problem
//		return false;
	for(int q = minChargeState; q <= maxChargeState; q++){
		double expectedMass = parentPeak->mass - neutralMass / 2;
		if(abs(childPeak->mass - expectedMass) < tolerance)
			return true;
	}
	return false;
}


	// Check if any neutral loss of interest was observed.
	for(int i = 0; i < p.neutralLosses.size(); i++)
		checkForNeutralMassLoss(MS2, parentPeak, p.neutralLosses[i].mass, p.neutralLosses[i].tolerance,
								p.neutralLosses[i].minCharge, p.neutralLosses[i].maxCharge, strdup(p.neutralLosses[i].color.c_str()));	//string vs char*



									void checkForNeutralMassLoss(ScanData* MS2, Peak* parentPeak, double neutralMass, double tolerance, int minChargeState, int maxChargeState, char* colorCode);
	bool isNeutralMassLoss(Peak* parentPeak, Peak* childPeak, double neutralMass, double tolerance, int minChargeState, int maxChargeState);

*/











/*



double Logic::idealIsolationWidth(ScanData* scanData, double mass){
	double closestBelow = scanData->scanRangeStart - 0.5;
	double closestAbove = scanData->scanRangeEnd + 0.5;
	for(int i = 0; i < scanData->numberOfMasses; i++){
		if(scanData->get_peak(i)->mass < mass)
			closestBelow = max(closestBelow, scanData->get_peak(i)->mass);
		if(scanData->get_peak(i)->mass > mass)
			closestAbove = min(closestAbove, scanData->get_peak(i)->mass);
	}
	double isolationWidth = min(mass - closestBelow, closestAbove - mass) * 2;
	isolationWidth = max(0.7, min(isolationWidth, 3.0));
	return isolationWidth;
}


*/