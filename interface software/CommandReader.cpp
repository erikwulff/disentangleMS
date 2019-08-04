
#include "StdAfx.h"
#include "CommandReader.h"
#include "MassSpecControl.h"
#include <fstream>
#include <sys/stat.h>
#include <sstream>

using namespace std;

CommandReader::CommandReader(CMassSpecControl* control, DataWriter* dataWriter, string path){
	this->filePath = path + "\\commands.txt";
	this->control = control;
	this->dataWriter = dataWriter;
	position = 0;
}

bool CommandReader::read(){
	if( dataWriter->onInstrumentCount > 20 )
		return false;

	ifstream file( filePath.c_str() );
	if( file.is_open() ){
		file.seekg(position);
		string line;
		while( getline(file, line) ){
			// If a full set of commands was just finished, give it a rest for now.
			// Store position so we can continue from here in the command file later.
//			if( line == "--- end of commands ---" ){
//				position = file.tellg();
//				file.close();
//				return true;
//			}

			// Parse command.
			stringstream ss(line);
			string setting;
			getline(ss, setting, ':');
			string parameter1;
			getline(ss, parameter1, ':');
			string parameter2;
			getline(ss, parameter2);

			// Handle command.
			if( setting == "setScanRange" )
				control->setScanRange( getDouble(parameter1), getDouble(parameter2) );
			else if( setting == "setScanMode" )
				control->setScanMode( getInt(parameter1) );
			else if( setting == "setParentMZ" )
				control->setParentMZ( getInt(parameter1), getDouble(parameter2) );
			else if( setting == "setIsolationWidth" )
				control->setIsolationWidth( getInt(parameter1), getDouble(parameter2) );
			else if( setting == "setActivationEnergy" )
				control->setActivationEnergy( getInt(parameter1), getDouble(parameter2) );
			else if( setting == "setActivationQ" )
				control->setActivationQ( getInt(parameter1), getDouble(parameter2) );
			else if( setting == "setActivationTime" )
				control->setActivationTime( getInt(parameter1), getDouble(parameter2) );
			else if( setting == "takeScan" ){
				if(control->takeScan()){
					dataWriter->onInstrumentCount++;
					position = file.tellg();
					file.close();
					return true;
				}
			} else if( setting == "setValveState" )
				control->setValveState( getInt(parameter1) );
			else if( setting == "setAnalyzer" )
				control->setAnalyzer( getInt(parameter1) );
			else if( setting == "setResolution" )
				control->setResolution( getUlong(parameter1) );
			else if( setting == "setReactionType" )
				control->setReactionType( getInt(parameter1), getInt(parameter2) );
			else {}
		}
		file.close();
	}
	return false;
}

int CommandReader::getInt( string str ){
	istringstream stm;
	stm.str( str );
	int output;
	stm >> output;
	return output;
}

double CommandReader::getDouble( string str ){
	istringstream stm;
	stm.str( str );
	double output;
	stm >> output;
	return output;
}

ULONG CommandReader::getUlong( string str ){
	istringstream stm;
	stm.str( str );
	ULONG output;
	stm >> output;
	return output;
}