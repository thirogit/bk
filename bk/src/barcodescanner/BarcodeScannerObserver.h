#ifndef __BARCODESCANNEROBSERVER__
#define __BARCODESCANNEROBSERVER__

#include <string>

class BarcodeScannerObserver
{
public:
	virtual void OnBarcode(const std::wstring& sBarcode) = 0;
	virtual void OnScannerConnected() = 0;
	virtual void OnScannerConnecing() = 0;
	virtual void OnScannerConnectionFailed() = 0;
	virtual void OnScannerDisconnected() = 0;
};

#endif