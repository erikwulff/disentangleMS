
#include "stdafx.h"

#include <windows.h>
#include <time.h>
#include "COMConnection.h"
#include "MassSpecControl.h"
#include "Quitter.h"
#include "DataWriter.h"
#include "CommandReader.h"
#include "Timer.h"
#include <sstream>

CWinApp theApp;
using namespace std;

double getDouble( string str ){
	istringstream stm;
	stm.str( str );
	double output;
	stm >> output;
	return output;
}

int main(int argc, char* argv[], char* envp[]){
	cout << "Please do not close this window." << endl;
	cout << "If you have questions, or if something seems to be misbehaving," << endl;
	cout << "you can call Erik at 215-796-7513.";
	Timer timer;
	double timeLimit = getDouble(argv[1]);

	if ( !AfxWinInit( ::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0 ) ){
		_tprintf( _T("Fatal Error: MFC initialization failed\n") );
		return 1;
	}
	CoInitialize(NULL);
	AfxEnableControlContainer();

	// Get path to executable.
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, sizeof(buffer));
	string file = string(buffer);
	string path = file.substr(0, file.find_last_of("\\/"));

	CMassSpecControl control = CMassSpecControl();
	Quitter quitter = Quitter(path);
	DataWriter dataWriter = DataWriter(&control, path);
	CommandReader commandReader = CommandReader(&control, &dataWriter, path);
	while( timer.seconds() < 20 && !quitter.read() ){
		control.discardScanData();
	}
	while( timer.minutes() < timeLimit && !quitter.read() ){
		commandReader.read();
		dataWriter.getData();
		dataWriter.writeData();
	}
	cout << "No longer taking instructions. ";
	cout << "Beginning to collect any remaining data off of the instrument." << endl;
	timer.reset();
	while( timer.minutes() < 0.1 && dataWriter.onInstrumentCount > 0 ){
		dataWriter.getData();
		dataWriter.writeData();
	}
	cout << "Finished collecting last of data from instrument." << endl;

	return 0;
}