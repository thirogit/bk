#ifndef __PASSSPORTSEGREGATIONDLG_H__
#define __PASSSPORTSEGREGATIONDLG_H__

#include <afxwin.h>
#include "../resource.h"
#include <array>
#include "CrossLayout.h"
#include "../ctrls/xcolorstatic/XColorStatic.h"
#include "../ctrls/layoutlistctrl/LayoutListCtrl.h"
#include "PassportCollection.h"
#include "../barcodescanner/ScannerEventListener.h"

class CPassportSegregationDlg : public CDialog, protected ScannerEventListener
{
public:
	CPassportSegregationDlg(CWnd* pParent = NULL); 
	virtual ~CPassportSegregationDlg();
	enum { IDD = IDD_PASSPORTSEGREGATION };

	bool AddCollection(const std::string& sLabel,const PassportCollection* pCollection);
	
protected:
	virtual BOOL OnInitDialog();
	virtual void OnBarcode(const std::string& sBarcode);
	virtual void OnScannerConnected();
	virtual void OnScannerDisconnected();
	virtual void DoDataExchange(CDataExchange* pDX); 	
protected:
	CXColorStatic m_SearchStatus;
	CXColorStatic m_Barcode;

protected:

	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif