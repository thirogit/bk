#ifndef __PASSSPORTSEARCHDLG_H__
#define __PASSSPORTSEARCHDLG_H__

#include <afxwin.h>
#include "../ctrls/xcolorstatic/XColorStatic.h"
#include "../resource.h"
#include "PassportCollection.h"
#include "SearchBucket.h"
#include "SearchBucketSummary.h"
#include "BucketListCtrl.h"
#include "../barcodescanner/ScannerEventListener.h"

class CPassportSearchDlg : public CDialog, protected ScannerEventListener
{
public:
	CPassportSearchDlg(const PassportCollection* pCollection,CWnd* pParent = NULL); 
	virtual ~CPassportSearchDlg();
	enum { IDD = IDD_PASSPORTSEARCH };

protected:
	virtual BOOL OnInitDialog();
	virtual void OnBarcode(const std::wstring& sBarcode);
	virtual void OnScannerConnected();
	virtual void OnScannerDisconnected();
protected:
	CXColorStatic m_SearchStatus;
	CXColorStatic m_Barcode;
	CSearchBucketSummary m_Summary;
	SearchBucket m_bucket;
	SearchBucketListCtrl m_bucketList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 	
	afx_msg void OnDestroy();	
	DECLARE_MESSAGE_MAP()
};

#endif