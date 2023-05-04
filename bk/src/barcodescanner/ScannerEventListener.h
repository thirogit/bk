#ifndef __SCANNEREVENTLISTENER__
#define __SCANNEREVENTLISTENER__

#include <string>

class ScannerEventListener
{
public:
	virtual void OnBarcode(const std::wstring& sBarcode) = 0;
	virtual void OnScannerConnected() = 0;
	virtual void OnScannerDisconnected() = 0;
};

#endif