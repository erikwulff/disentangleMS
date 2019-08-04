
#include "StdAfx.h"
#include "DataWriter.h"
#include "MassSpecControl.h"
#include "Timer.h"
#include <fstream>
#include <sstream>

using namespace std;

DataWriter::DataWriter(CMassSpecControl* control, string path){
	this->filePath = path + "\\data.txt";
	this->onInstrumentCount = 0;
	this->control = control;
}

// If <onInstrumentCount> indicates that there are no scans on instrument, does nothing.
// Otherwise, tries to get scan data from instrument.
// If it gets a scan, adds the data to <scanDataQueue>.
bool DataWriter::getData(){
	if( scanDataQueue.size() > 50 )
		cout << "Too much scan data waiting to be written!" << endl;
	if(onInstrumentCount < 1)
		return false;

	int scanNumber;
	LTQ_INST_Analyzer analyzer = LTQ_INST_Analyzer_NumItems;
	float IT, ST;
	double totalIntensity;
	unsigned long numberOfPeaks;
	double* mzValues = NULL;
	double* intensities = NULL;
	if( control->getScanData(scanNumber, IT, ST, analyzer, totalIntensity, numberOfPeaks, mzValues, intensities) ){
		scanDataQueue.push( new ScanData(scanNumber, IT, ST, numberOfPeaks, mzValues, intensities, analyzer) );
		onInstrumentCount--;
		return true;
	}
	return false;
}

// If there is no data on <scanDataQueue> (data already collected from instrument but not yet written to file),
// does nothing.
// Otherwise, pops data off scanDataQueue and writes it to file.
bool DataWriter::writeData(){
	if(scanDataQueue.size() < 1)
		return false;

	ofstream file;
	file.open( filePath.c_str(), ios::app );

	if ( file.is_open() ){
		stringstream buffer;
		ScanData* scanData = scanDataQueue.front();
		scanDataQueue.pop();

		buffer << scanData->scanNumber << " ";
		buffer << scanData->IT << " ";
		buffer << scanData->ST << " ";
		buffer << scanData->numberOfPeaks << " ";
		buffer << scanData->analyzer << endl;
		for( int i = 0; i < scanData->numberOfPeaks; i++ )
			buffer << scanData->mzValues[i] << " " << scanData->intensities[i] << endl;
		buffer << "---end of data---" << endl;
		
		file << buffer.str();

		delete scanData->mzValues, scanData->intensities;
		delete scanData;

		file.close();
	}
	return true;
}