// InstControlEventSink.cpp : implementation file
// This is where you would implement the event handlers for various messages the mass spec sends to the computers

#include "stdafx.h"
#include "MassSpecControl.h"
#include "InstControlEventSink.h"

// CInstControlEventSink

IMPLEMENT_DYNAMIC(CInstControlEventSink, CCmdTarget)
CInstControlEventSink::CInstControlEventSink() : m_pParentDialog(NULL)
{
	EnableAutomation();
}

CInstControlEventSink::~CInstControlEventSink()
{
}


void CInstControlEventSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CInstControlEventSink, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CInstControlEventSink, CCmdTarget)
	DISP_FUNCTION_ID(CInstControlEventSink, "OnTurboItemStatus", 1, OnTurboItemStatus, VT_EMPTY, VTS_I4 VTS_VARIANT)
	DISP_FUNCTION_ID(CInstControlEventSink, "OnSyringeItemStatus", 2, OnSyringeItemStatus, VT_EMPTY,VTS_I4 VTS_VARIANT)
	DISP_FUNCTION_ID(CInstControlEventSink, "OnInstrumentStatus", 3, OnInstrumentStatus, VT_EMPTY, VTS_I4 VTS_VARIANT)
	DISP_FUNCTION_ID(CInstControlEventSink, "OnShutdown", 4, OnShutdown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CInstControlEventSink, "OnAutoScanDataReady", 5, OnAutoScanDataReady, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CInstControlEventSink, "OnAccessState", 6, OnAccessState, VT_EMPTY, VTS_UI4)
END_DISPATCH_MAP()

// Note: we add support for IID_IInstControlEventSink to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {6E215968-8438-47B5-B5EA-D45C1CC677AB}
//static const IID IID_IInstControlEventSink =
//{ 0x6E215968, 0x8438, 0x47B5, { 0xB5, 0xEA, 0xD4, 0x5C, 0x1C, 0xC6, 0x77, 0xAB } };

BEGIN_INTERFACE_MAP(CInstControlEventSink, CCmdTarget)
	INTERFACE_PART(CInstControlEventSink, __uuidof(_ILTQInstCtrlEvents), Dispatch)
END_INTERFACE_MAP()


void CInstControlEventSink::OnDeviceStatus(enum LTQ_INST_Devices eDevice, float fValue)
{ 
}

void CInstControlEventSink::OnAnalogStatus(enum LTQ_INST_Analog eAnalog, float fValue)
{
}

void CInstControlEventSink::OnTurboItemStatus(enum LTQ_INST_TurboItem eTurboItem, const VARIANT FAR& vValue)
{
}

void CInstControlEventSink::OnSyringeItemStatus(enum LTQ_INST_SyringeItem eSyringeItem, const VARIANT FAR& vValue)
{
}

void CInstControlEventSink::OnInstrumentStatus(enum LTQ_INST_InstrumentStateItem eInstrumentStateItem, const VARIANT FAR& vValue)
{
}

void CInstControlEventSink::OnShutdown(void)
{
}

void CInstControlEventSink::OnAutoScanDataReady()
{
//	m_pParentDialog->UpdateSpectrum();
}

void CInstControlEventSink::OnAccessState(unsigned long lCount)
{
}

// CInstControlEventSink message handlers
