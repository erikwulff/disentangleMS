
#include "StdAfx.h"
#include "MassSpecControl.h"
#include "Timer.h"

using namespace std;

CMassSpecControl::CMassSpecControl(){
	//Initializes event handler. I do not use this.
	m_pInstCtrlSink = new CInstControlEventSink();
	m_pInstCtrlSink->SetParentDialog(this);
	
	//Create instance of COM object
	if(FAILED( m_InstCtrl.CreateInstance(_T("LTQInstControl.LTQInstCtrl"), NULL)))
		quit("Failed to create LTQ Instrument Control object.");
	
	//Connect to instrument event sink
	if(FAILED(AtlAdvise(m_InstCtrl, m_pInstCtrlSink->GetInterface(&IID_IUnknown), __uuidof(_ILTQInstCtrlEvents), &m_dwCookie)))
		quit("Failed to connect LTQ Instrument Control object event sink.");

	//Connect to instrument
	if(FAILED(m_InstCtrl->raw_ConnectToInstrument()))
		quit("Failed to connect to instrument.");

	//Set autoscan mode to off, prevents instrument from scanning automatically once instrument is turned on
	if(FAILED(m_InstCtrl->raw_SetAutoScanMode(LTQ_INST_AUTOSCAN_OFF)))
		quit("Failed to set auto scan mode to off.");

	//Turn mass spec on
	if(FAILED(m_InstCtrl->raw_SetSysState(LTQ_INST_STATE_ON)))
		quit("Failed to turn mass spec on.");
	
	//Lock access to the instrument so that others cannot control it
	long lCount;
	if(FAILED(m_InstCtrl->raw_LockAccess(&lCount)))
		quit("Failed to lock access to instrument.");

	//Enable asynchronous scanning (when a scan is requested, control is returned to this code before the scan is done)
	if(FAILED(m_InstCtrl->put_SyncScanning(VARIANT_FALSE)))
		quit("Failed to set scanning to asynchronous mode.");
}

void CMassSpecControl::releaseControl(){
	if(m_InstCtrl){
		cout << "Beginning to unlock instrument access." << endl;
		long lCount;
		Timer timer;
		do
			m_InstCtrl->raw_UnlockAccess(&lCount);
		while(lCount > 0 && timer.seconds() <= 60);
		if(timer.seconds() > 60)
			cout << "ERROR: There was a problem unlocking instrument access!" << endl;
		else
			cout << "Done unlocking instrument access." << endl;

		if(FAILED(m_InstCtrl->raw_SetSysState(LTQ_INST_STATE_STANDBY)))
			cout << "WARNING: Failed to set instrument to Standby.";

		if(FAILED(m_InstCtrl->raw_SetAutoScanMode(LTQ_INST_AUTOSCAN_ON)))
			cout << "WARNING: Failed to turn auto scan on.";

		if(m_dwCookie != 0){
			AtlUnadvise(m_InstCtrl, __uuidof(_ILTQInstCtrlEvents), m_dwCookie);
			m_dwCookie = 0;
		}

		if(m_pInstCtrlSink != NULL){
			delete m_pInstCtrlSink;
			m_pInstCtrlSink = NULL;
		}
	}
}

void CMassSpecControl::quit(char* error){
	cout << "ERROR: " << error << endl;
	releaseControl();
	exit(1);
}

CMassSpecControl::~CMassSpecControl(){
	cout << "Beginning to release control." << endl;
	releaseControl();
	cout << "Done releasing control." << endl;
}

bool CMassSpecControl::getScanData(int &scanNumber, float &IT, float &ST, LTQ_INST_Analyzer &analyzer, double &totalIntensity, 
		unsigned long &numberOfPeaks, double* &mzValues, double* &intensities){

	if( FAILED( m_InstCtrl->get_TrailerScanNumber(&scanNumber) ) )
		return false;

	if( FAILED( m_InstCtrl->get_TrailerIonTime(&IT) ) )
		cout << "WARNING: Could not get trailer ion time." << endl;
	if( FAILED( m_InstCtrl->get_TrailerElapsedScanTime(&ST) ) )
		cout << "WARNING: Could not get trailer scan time." << endl;
	if( FAILED( m_InstCtrl->get_TrailerIntegIntensity(&totalIntensity) ) )
		cout << "WARNING: Could not get trailer total intensity." << endl;
	if( FAILED( m_InstCtrl->raw_GetMassIntensityDataSize(&numberOfPeaks) ) )
		cout << "WARNING: Could not get mass intensity data size." << endl;

	if( FAILED( m_InstCtrl->get_TrailerAnalyzer( &analyzer ) ) )
		cout << "WARNING: Could not get analyzer from trailer data." << endl;

	mzValues = new double[numberOfPeaks + 1];
	intensities = new double[numberOfPeaks + 1];
	unsigned long fetchedsize;
	if(numberOfPeaks > 0)
		if( FAILED( m_InstCtrl->raw_GetMassIntensityData(0, numberOfPeaks, mzValues, intensities, &fetchedsize) ) ){
			cout << "WARNING: Could not fetch mass data.";
			numberOfPeaks = 0;
		}

	if( SUCCEEDED( m_InstCtrl->raw_ReleaseScanData() ) )
		return true;
	cout << "WARNING: Scan data could not be released." << endl;
	return false;
}

bool CMassSpecControl::discardScanData(){
	int scanNumber = 0;
	if( FAILED( m_InstCtrl->get_TrailerScanNumber(&scanNumber) ) )
		return false;
	if( SUCCEEDED( m_InstCtrl->raw_ReleaseScanData() ) )
		return true;
	cout << "WARNING: Scan data could not be released." << endl;
	return false;
}

bool CMassSpecControl::takeScan(){
	// Checks if scan definition is valid, avoids applying it if it is not.
	// This does not seem to add significant delay, but I should check more thuroughly.
	VARIANT_BOOL bScanDefValid(false);
	BSTR bstrError(NULL);
	HRESULT hr(0);
	LTQ_INST_SCANDEF_ERROR scandefError;
	hr = m_InstCtrl->raw_CheckScanDefValidity( &scandefError, &bstrError, &bScanDefValid );
	if( FAILED(hr) ){
		AfxMessageBox(bstrError);
		SysFreeString(bstrError);
		return false;
	}
	SysFreeString(bstrError);

	HRESULT result = m_InstCtrl->raw_TakeAScanAfterSettingScanDef();
	if( FAILED(result) ){
		cout << "WARNING: Failed to set scan definition and take a scan." << endl;
		return false;
	}
	return true;
}

bool CMassSpecControl::setScanRange(double scanRangeStart, double scanRangeEnd){
	if( scanRangeStart < 50.0 || scanRangeEnd > 2000.0 || scanRangeEnd < scanRangeStart + 1.0 ){ //confirmed
		cout << "WARNING: Request to set scan range to ";
		cout << scanRangeStart << " - " << scanRangeEnd << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->raw_SetScanDefMassRange(0, scanRangeStart, scanRangeEnd);
	if( FAILED(result) ){
		cout << "WARNING: Failed to set scan range to ";
		cout << scanRangeStart << " - " << scanRangeEnd << " aborted." << endl;
		return false;
	}
	return true;
}

bool CMassSpecControl::setScanMode(int scanMode){
	if( scanMode < 0 || scanMode > 9 ){
		cout << "WARNING: Request to set scan mode to ";
		cout << scanMode << " (MS" << (scanMode + 1) << ") aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_ScanDefScanMode( (LTQ_INST_ScanMode) scanMode );
	if( FAILED(result) ){
		cout << "WARNING: Failed to set scan mode to ";
		cout << scanMode << " (MS" << (scanMode + 1) << ")." << endl;
		return false;
	}
	currentScanMode = scanMode;
	return true;
}

bool CMassSpecControl::setParentMZ(int level, double parentMZ){
	if( level < 0 || level >= currentScanMode || parentMZ < 50 || parentMZ > 2000 ){  //confirmed
		cout << "WARNING: Request to set parent mass for scan mode ";
		cout << level << " to " << parentMZ << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_ScanDefReactionPrecursorMass(level, parentMZ);
	if( FAILED(result) ){
		cout << "WARNING: Failed to set parent mass for scan mode ";
		cout << level << " to " << parentMZ << "." << endl;
		return false;
	}
	return true;
}

bool CMassSpecControl::setIsolationWidth(int level, double isolationWidth){
	if( level < 0 || level >= currentScanMode || isolationWidth < 0.1 || isolationWidth > 100 ){ //confirmed
		cout << "WARNING: Request to set isolation width for scan mode ";
		cout << level << " to " << isolationWidth << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_ScanDefReactionIsolationWidth(level, isolationWidth);
	if( FAILED(result) ){
		cout << "WARNING: Failed to set isolation width for scan mode ";
		cout << level << " to " << isolationWidth << "." << endl;
		return false;
	}
	return true;
}

bool CMassSpecControl::setActivationEnergy(int level, double activationEnergy){
	if( level < 0 || level >= currentScanMode || activationEnergy < 0 || activationEnergy > 400 ){ //think this is actual range
		cout << "WARNING: Request to set activation energy for scan mode ";
		cout << level << " to " << activationEnergy << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_ScanDefReactionCurrInfoEnergy(level, activationEnergy);
	if( FAILED(result) ){
		cout << "WARNING: Failed to set activation energy for scan mode ";
		cout << level << " to " << activationEnergy << "." << endl;
		return false;
	}
	return true;
}

bool CMassSpecControl::setActivationQ(int level, float activationQ){ //confirmed
	if( level < 0 || level >= currentScanMode || activationQ < 0.01 || activationQ > 0.9 ){
		cout << "WARNING: Request to set activation Q for scan mode ";
		cout << level << " to " << activationQ << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_ScanDefReactionActivationQ(level, activationQ);
	if( FAILED(result) ){
		cout << "WARNING: Failed to set activation Q for scan mode ";
		cout << level << " to " << activationQ << "." << endl;
		return false;
	}
	return true;
}

bool CMassSpecControl::setActivationTime(int level, double activationTime){ //confirmed
	if( level < 0 || level >= currentScanMode || activationTime < 0.03 || activationTime > 60000 ){
		cout << "WARNING: Request to set activation time for scan mode ";
		cout << level << " to " << activationTime << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_ScanDefReactionCurrInfoTime(level, activationTime);
	if( FAILED(result) ){
		cout << "WARNING: Failed to set activation time for scan mode ";
		cout << level << " to " << activationTime << "." << endl;
		return false;
	}
	return true;
}

bool CMassSpecControl::setValveState(int newState){
	if( newState < 0 || newState > 1 ){
		cout << "WARNING: Attempt to set divert valve state to ";
		cout << newState << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_DivertInjectState( (LTQ_INST_DIVERT_INJECT)newState );
	if( FAILED(result) ){
		cout << "WARNING: Failed to set divert valve state to ";
		cout << newState << "." << endl;
		return false;
	}
	return true;	
}

bool CMassSpecControl::setAnalyzer(int analyzer){
	if( analyzer < 0 || analyzer > 1 ){
		cout << "WARNING: Attempt to set analyzer to ";
		cout << analyzer << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_ScanDefAnalyzer( (LTQ_INST_Analyzer)analyzer );
	if( FAILED(result) ){
		cout << "WARNING: Failed to set analyzer to ";
		cout << analyzer << "." << endl;
		return false;
	}
	return true;	
}

bool CMassSpecControl::setResolution(ULONG resolution){
	if( resolution != 7500 && resolution != 15000 && resolution != 30000 && 
		resolution != 60000 && resolution != 100000 ){	// confirmed
		cout << "WARNING: Attempt to set resolution to ";
		cout << resolution << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_ScanDefResolution(resolution);
	if( FAILED(result) ){
		cout << "WARNING: Failed to set resolution to ";
		cout << resolution << "." << endl;
		return false;
	}
	return true;	
}

// I think there should be an additional limitation for when set to HCD, has to be last.
// This thing is not set up properly. Need to make sure it is before using HCD.
bool CMassSpecControl::setReactionType(int level, int type){
	if( level < 0 || level >= currentScanMode || ( type != 0 && type != 3 ) ){
		cout << "WARNING: Request to set reaction type for scan mode ";
		cout << level << " to " << type << " aborted." << endl;
		return false;
	}
	HRESULT result = m_InstCtrl->put_ScanDefReactionType(level, (LTQ_INST_ReactionType)type);
	if( FAILED(result) ){
		cout << "WARNING: Failed to set reaction type for scan mode ";
		cout << level << " to " << type << "." << endl;
		return false;
	}
	return true;	
}