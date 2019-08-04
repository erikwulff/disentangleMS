
#include "StdAfx.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include "TestInstrument.h"
#include "Timer.h"
#include <queue>

using namespace std;

int randInt(int from, int to){
	int largeRand = rand() * RAND_MAX + rand();
	return largeRand % (to - from) + from;
}

double randDouble(int from, int to){
	int largeRand = rand() * RAND_MAX + rand();
	return (largeRand % ((to - from) * 100)) / 100.0 + from;
}


TestInstrument::TestInstrument(void){
	time = Timer();
	numberQueued = 0;
}

TestInstrument::~TestInstrument(void){}

bool TestInstrument::copySettings(Spectrum* s){
	setScanRange(s->scanRangeStart, s->scanRangeEnd);
	Spectrum* parent = s;
	while(true){
		setParentMZ(parent->scanMode, parent->parentMass);
		setIsolationWidth(parent->scanMode, parent->isolationWidth);
		setActivationEnergy(parent->scanMode, parent->activationEnergy);
		setActivationQ(parent->scanMode, parent->activationQ);
		setActivationTime(s->scanMode, s->activationTime);

		if(parent->getParentPeak() == NULL)
			break;
		parent = parent->getParentPeak()->getSpectrum();
	}
	setScanMode(s->scanMode);
	Settings set = Settings();
	set.from = s->scanRangeStart;
	set.to = s->scanRangeEnd;
	set.parentMZ = parent->parentMass;
	set.isolationWidth = parent->isolationWidth;
	set.spectrum = s;
	scanQueue.push(set);
	return true;
}

bool TestInstrument::setScanRange(double scanRangeStart, double scanRangeEnd){
	return true;
}

bool TestInstrument::setScanMode(int scanMode){
	return true;
}

bool TestInstrument::setParentMZ(int level, double parentMZ){
	return true;
}

bool TestInstrument::setIsolationWidth(int level, double isolationWidth){
	return true;
}

bool TestInstrument::setActivationEnergy(int level, double activationEnergy){
	return true;
}

bool TestInstrument::setActivationQ(int level, double activationQ){
	return true;
}

bool TestInstrument::setActivationTime(int level, double activationTime){
	return true;
}

bool TestInstrument::takeScan(void){
	if(numberQueued == 0)
		time.reset();
	numberQueued++;
	return true;
}

bool TestInstrument::getScanData(int &scanNumber, float &IT, float &ST, double &totalIntensity, 
		unsigned long &numberOfPeaks, double* &mzValues, double* &intensities){

	// ensure that the instrument would have had enough time to complete the scan
	double timePerScan = 0.2;
	if( time.seconds() < timePerScan )
		return false;
	time.setInSeconds( max(time.seconds() - timePerScan, 0) );

	// ensure that there is a queued scan to return
	if(numberQueued == 0)
		return false;
	numberQueued--;

	// set IT and ST
	IT = rand() % 200;
	ST = rand() % 200;

	// Set spectral data
	Settings settings = scanQueue.front();
	scanQueue.pop();
	PeakSet* randomSpectrum = spectrumGenerator.generate(settings.spectrum);
	numberOfPeaks = randomSpectrum->size();
	mzValues = new double[max(numberOfPeaks, 1)];
	intensities = new double[max(numberOfPeaks, 1)];
	totalIntensity = 0;
	for(int i = 0; i < randomSpectrum->size(); i++){
		mzValues[i] = randomSpectrum->getPeak(i)->getMZ();
		intensities[i] = randomSpectrum->getPeak(i)->getIntensity();
		totalIntensity += intensities[i];
	}
	delete randomSpectrum;

	return true;
}

bool TestInstrument::set_valveState(LTQ_INST_DIVERT_INJECT newState){
	return true;
}
