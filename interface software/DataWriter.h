#pragma once
#include "MassSpecControl.h"
#include <fstream>
#include <queue>

using namespace std;

struct ScanData {
	int scanNumber;
	float IT;
	float ST;
	unsigned long numberOfPeaks;
	double* mzValues;
	double* intensities;
	LTQ_INST_Analyzer analyzer;
	ScanData(int sn, float it, float st, unsigned long nop, double* m, double* i, LTQ_INST_Analyzer an){
		scanNumber = sn;
		IT = it;
		ST = st;
		numberOfPeaks = nop;
		mzValues = m;
		intensities = i;
		analyzer = an;
	}
};

class DataWriter{
	CMassSpecControl* control;
	queue<ScanData*> scanDataQueue;		// Finished scans from instrument waiting to be written to file
	string filePath;
	
public:
	int onInstrumentCount;
	
	DataWriter(CMassSpecControl* control, string path);
	bool getData();
	bool writeData();
};

