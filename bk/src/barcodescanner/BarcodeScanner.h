#ifndef __BARCODESCANNER__
#define __BARCODESCANNER__

#include <string>
#include <afxwin.h>

#include "symbolscanner.h"
#include "BarcodeScannerObserver.h"
#include <functional>

class BarcodeScanner : public CWnd
{
public:
	BarcodeScanner();
	bool Create(BarcodeScannerObserver *pObserver);
	bool IsOnline();
	void Connect();
	void Disconnect();
private:

	template<typename F>
	void InvokeLater(F& functor)
	{
		std::function<void ()> fn = functor;
		InvokeLater(fn);
	}

	void InvokeLater(std::function<void ()>& fn);

	bool RegisterWndClass();
	void OnUSBScannerPluggedIn();
	void OnUSBScannerPluggedOut();
	bool IsUSBScannerPluggedIn();
	bool CompareGUID(const GUID *pGUID1,const GUID *pGUID2);
	bool m_bScannerPluggedIn;

	CSymbolScanner m_scanner;
	bool m_bOnline;
	BarcodeScannerObserver *m_pObserver;
	HDEVNOTIFY m_hDevNotify;
protected:
	afx_msg void OnDataEventScanner(long lStatus);
	afx_msg LRESULT OnUSBDeviceChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInvokeLater(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
	
};

#endif