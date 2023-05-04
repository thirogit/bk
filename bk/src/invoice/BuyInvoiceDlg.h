#ifndef __BUYINVOICEDLG_H__
#define __BUYINVOICEDLG_H__

#include "BaseInvoiceDlg.h"
#include "../header/HeaderLayoutConfiguration.h"

class CBuyInvoiceDlg : public CBaseInvoiceDlg
{	
public:
	CBuyInvoiceDlg(CWnd* pParent = NULL);
	virtual ~CBuyInvoiceDlg();
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

