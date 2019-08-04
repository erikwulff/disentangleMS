#pragma once
#include "MassSpecControl.h"
#include <fstream>
#include <string>
#include "DataWriter.h"

using namespace std;

class Quitter{
	string filePath;

public:
	Quitter(string path);
	bool read();
};

