#ifndef __DOCSELECTDLG_H__
#define __DOCSELECTDLG_H__

#include <afxwin.h>
#include "../resource.h"
#include "../SessionAwareDialog.h"
#include "../ctrls/contentlistctrl/ContentListCtrl.h"
#include "DocSelectInterface.h"

class DocSelectDlg : public CSessionAwareDialog
{
public:
	DocSelectDlg(DocSelectInterface *pInterface,CWnd* pParent = NULL);
	enum { IDD = IDD_DOCSELECT };
	void SetHeader(const ListHeaderDescriptor& header);
protected:
	CContentListCtrl m_CandidatesList;	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	
	void UpdateCount();
protected:
	DocSelectInterface *m_pInterface;

	afx_msg void OnPickRow();
	afx_msg void OnUnpickRow();
			
	DECLARE_MESSAGE_MAP()
};

#endif