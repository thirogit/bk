#ifndef __SCANNERSTATUSBTN_H__
#define __SCANNERSTATUSBTN_H__

#include <afxwin.h>
#include "../ScannerConnectionStatus.h"
#include "../../ctrls/pngbutton/PngButton.h"
#include "../ScannerEventListener.h"

class CScannerStatusBtn : public CPngButton, public ScannerEventListener
{
public:
	CScannerStatusBtn();
	virtual ~CScannerStatusBtn();
	void SetStatus(ScannerConnectionStatus scannerStatus);
private:
	CPng m_yesBarcodeImg;
	CPng m_noBarcodeImg;
	ScannerConnectionStatus m_scannerStatus;
protected:
	virtual void PreSubclassWindow();
	virtual void DrawImage(Gdiplus::Graphics& graphics, const CRect& drawRect);
	virtual void OnBarcode(const std::wstring& sBarcode);
	virtual void OnScannerConnected();
	virtual void OnScannerDisconnected();
	void Init();
protected:
	
	DECLARE_MESSAGE_MAP()	
};

#endif 
