#pragma once

#include "InstControlEventSink.h"
#include "Spectrum.h"
#include "Timer.h"
#include "SeriePredictor.h"
#include <queue>
#include "SpectrumGenerator.h"
#import "C:\\Users\\Wulff\\Documents\\Visual Studio 2008\\Projects\\SMS\\LTQInstControl.dll"
using namespace LTQInstControlLib;

struct Settings{
	double from;
	double to;
	double parentMZ;
	double isolationWidth;
	Spectrum* spectrum;
};

class TestInstrument{
	int numberQueued;
	Timer time;
	queue<Settings> scanQueue;
	SeriePredictor seriePredictor;
	SpectrumGenerator spectrumGenerator;


public:
	TestInstrument(void);
	~TestInstrument(void);
	bool copySettings(Spectrum* spectrum);
	bool setScanRange(double scanRangeStart, double scanRangeEnd);
	bool setScanMode(int scanMode);
	bool setParentMZ(int level, double parentMZ);
	bool setIsolationWidth(int level, double isolationWidth);
	bool setActivationEnergy(int level, double activationEnergy);
	bool setActivationQ(int level, double activationQ);
	bool setActivationTime(int level, double activationTime);
	bool takeScan(void);
	bool getScanData(int &scanNumber, float &IT, float &ST, double &totalIntensity, 
		unsigned long &numberOfPeaks, double* &mzValues, double* &intensities);
	bool set_valveState(LTQ_INST_DIVERT_INJECT newState);


};

