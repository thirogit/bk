#ifndef __SELLINVOICEDLG_H__
#define __SELLINVOICEDLG_H__

#include "BaseInvoiceDlg.h"
#include "../header/HeaderLayoutConfiguration.h"

class CSellInvoiceDlg : public CBaseInvoiceDlg
{	
public:
	CSellInvoiceDlg(CWnd* pParent = NULL);
	virtual ~CSellInvoiceDlg();
protected:
	HeaderLayoutConfiguration* m_itemsHdrLayout;
	HeaderLayoutConfiguration* m_deductionsHdrLayout;
	HeaderLayoutConfiguration* m_docsHdrLayout;
protected:
		
	virtual BOOL OnInitDialog();
	

	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

#endif

