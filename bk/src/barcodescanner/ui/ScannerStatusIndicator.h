#ifndef __SCANNERSTATUSINDICATOR_H__
#define __SCANNERSTATUSINDICATOR_H__

#include <afxwin.h>
#include "../ScannerConnectionStatus.h"
#include "../../image/Png.h"

class CScannerStatusIndicator : public CStatic
{
public:
	CScannerStatusIndicator();
	virtual ~CScannerStatusIndicator();
	void SetStatus(ScannerConnectionStatus scannerStatus);
private:
	CPng m_yesBarcodeImg;
	CPng m_noBarcodeImg;
	CPng m_yesBarcodeDisabledImg;
	CPng m_noBarcodeDisabledImg;
	ScannerConnectionStatus m_scannerStatus;
protected:
	virtual void PreSubclassWindow();
	void Init();
protected:
	afx_msg void OnPaint();
	afx_msg void OnEnable(BOOL bEnable);
	
	DECLARE_MESSAGE_MAP()	
};

#endif 
