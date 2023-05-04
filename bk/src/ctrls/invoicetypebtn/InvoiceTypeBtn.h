#ifndef __INVOICETYPEBTN_H__
#define __INVOICETYPEBTN_H__

#include <data\InvoiceType.h>
#include <afxwin.h>

class CInvoiceTypeBtn : public CStatic
{
public:
	CInvoiceTypeBtn();
	virtual ~CInvoiceTypeBtn();

	void SetInvoiceType(InvoiceType type);
	InvoiceType GetInvoiceType() const;
	
protected:
	virtual void PreSubclassWindow();
	void CreateButtons();
	void CheckButtons();
	
	InvoiceType m_type;
	CToolBarCtrl m_Btns;

	afx_msg void OnRegular();
	afx_msg void OnLump();


	DECLARE_MESSAGE_MAP()
};


#endif

