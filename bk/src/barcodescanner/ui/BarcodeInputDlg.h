#ifndef __BARCODEINPUTDLG_H__
#define __BARCODEINPUTDLG_H__

#include "../ScannerEventListener.h"
#include "../../ctrls/xcolorstatic/XColorStatic.h"
#include "../../resource.h"
#include <exception/TagException.h>

struct tag_BarcodeValidationException{};
typedef TagException<tag_BarcodeValidationException> BarcodeValidationException;

class CBarcodeInputDlg : public CDialog, public ScannerEventListener
{
	DECLARE_DYNAMIC(CBarcodeInputDlg)
protected:
	CBarcodeInputDlg(CWnd* pParent = NULL);
	virtual ~CBarcodeInputDlg();
	enum { IDD = IDD_BCINPUT };
		
protected:

	virtual void ShowBarcodePrompt();
	void SetStatusText(const std::wstring& status);
	void ClearStatusText();
		
	std::wstring m_sEntryText;
	CXColorStatic m_entryText;
	CXColorStatic m_barcodeValue;
	CXColorStatic m_Status;
protected:
	virtual void OnBarcode(const std::wstring& sBarcode);
	virtual void OnScannerConnected();
	virtual void OnScannerDisconnected();

	virtual void ReceiveInput(const std::wstring& sBarcode) = 0;
	virtual std::wstring GetEntryText() = 0;	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif