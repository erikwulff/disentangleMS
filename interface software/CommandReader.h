#pragma once
#include "MassSpecControl.h"
#include <fstream>
#include <string>
#include "DataWriter.h"

using namespace std;

class CommandReader{
	CMassSpecControl* control;
	int position;
	DataWriter* dataWriter;
	string filePath;

public:
	CommandReader(CMassSpecControl* control, DataWriter* dataWriter, string path);
	bool read();
	int getInt( string str );
	double getDouble( string str );
	ULONG getUlong( string str );
};

