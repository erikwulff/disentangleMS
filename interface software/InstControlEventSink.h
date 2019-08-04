#pragma once
//This is an event sink for various messages the mass spec can send to the computer, I haven't found a use for it

//Generates .tlh and .tli files from dll, then replaces #import with #include ..\Debug\LTQInstControl.tlh
#import "C:\Users\Wulff\Documents\Visual Studio 2008\Projects\SMS\LTQInstControl.dll"
using namespace LTQInstControlLib;

// CInstControlEventSink command target
class CInstControlEventSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CInstControlEventSink)

public:
	CInstControlEventSink();
	virtual ~CInstControlEventSink();

	virtual void OnFinalRelease();

	void SetParentDialog(class CMassSpecControl* pDlg){ ASSERT(pDlg); m_pParentDialog = pDlg;}

protected:
	DECLARE_MESSAGE_MAP()


	afx_msg void OnDeviceStatus(enum LTQ_INST_Devices eDevice, float fValue);
	afx_msg void OnAnalogStatus(enum LTQ_INST_Analog eAnalog, float fValue);
	afx_msg void OnTurboItemStatus(enum LTQ_INST_TurboItem eTurboItem, const VARIANT FAR& vValue);
	afx_msg void OnSyringeItemStatus(enum LTQ_INST_SyringeItem eSyringeItem, const VARIANT FAR& vValue);
	afx_msg void OnInstrumentStatus(enum LTQ_INST_InstrumentStateItem eInstrumentStateItem, const VARIANT FAR& vValue);
	afx_msg void OnShutdown(void);
	afx_msg void OnAutoScanDataReady();
	afx_msg void OnAccessState(unsigned long lCount);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
	class CMassSpecControl* m_pParentDialog;

};


