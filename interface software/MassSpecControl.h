#pragma once

#include "InstControlEventSink.h"

//Imports the LTQInstCtrl library, creates LTQInstControl.tlh and LTQInstControl.tli 
//and puts them in the .\Debug directory, this command is Visual Studio specific
#import "C:\\Users\\Wulff\\Documents\\Visual Studio 2008\\Projects\\SMS\\LTQInstControl.dll"
using namespace LTQInstControlLib;

class CMassSpecControl{
	ILTQInstCtrlPtr m_InstCtrl;									// Pointer to class interface of COM Object
	CInstControlEventSink* m_pInstCtrlSink;						// User-defined event handler
	DWORD m_dwCookie;											// Used by LTQInstCtrl object

	int currentScanMode;										// To prevent setting settings for levels
																// above current scan mode.

	void releaseControl(void);									// Releases control of the mass spec
	void quit(char* error);

public:
	CMassSpecControl();
	~CMassSpecControl();
	bool setScanRange(double scanRangeStart, double scanRangeEnd);
	bool setScanMode(int scanMode);
	bool setParentMZ(int level, double parentMZ);
	bool setIsolationWidth(int level, double isolationWidth);
	bool setActivationEnergy(int level, double activationEnergy);
	bool setActivationQ(int level, float activationQ);
	bool setActivationTime(int level, double activationTime);
	bool takeScan();
	bool getScanData(int &scanNumber, float &IT, float &ST, LTQ_INST_Analyzer &analyzer, double &totalIntensity, 
		unsigned long &numberOfPeaks, double* &mzValues, double* &intensities);
	bool discardScanData();
	bool setValveState(int newState);
	bool setAnalyzer(int analyzer);
	bool setResolution(ULONG resolution);
	bool setReactionType(int level, int type);
};

