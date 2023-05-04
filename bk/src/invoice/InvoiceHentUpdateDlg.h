#ifndef __INVOICEHENTUPDATEDLG_H__
#define __INVOICEHENTUPDATEDLG_H__

#include <data\impl\InvoiceHent.h>
#include "../resource.h"
#include "../ctrls/coloredit/ColorEdit.h"
#include "../ctrls/HentNoEdit.h"
#include "../ctrls/AccountNoEdit.h"
#include "../ctrls/dateedit/dateedit.h"
#include "../ctrls/stdedit/stdedit.h"

#define DIFFERENCE_COLOR (RGB(255,127,39))

class CInvoiceHentUpdateDlg : public CDialog
{	
	class InvoiceHentSide
	{
	public:
		CColorEdit<CStdEdit> name;
		CColorEdit<CStdEdit> street;
		CColorEdit<CStdEdit> POBox;
		CColorEdit<CStdEdit> city;
		CColorEdit<CStdEdit> zip;
		CColorEdit<CHentNoEdit> hentNo;
		CColorEdit<CStdEdit> fiscalNo;
		CColorEdit<CAccountNoEdit> accountNo;
		CColorEdit<CStdEdit> bank;
		CColorEdit<CStdEdit> personalNo;
		CColorEdit<CStdEdit> personalIdNo;
		CColorEdit<CDateEdit> personalIdIssueDt;
		CColorEdit<CStdEdit> personalIdIssuePost;
	};

public:
	CInvoiceHentUpdateDlg(const InvoiceHent& toUpdate,const InvoiceHent& reference );   
	enum { IDD = IDD_INVOICEHENT };	
	virtual ~CInvoiceHentUpdateDlg();	
	const InvoiceHent& GetUpdatedHent() const;

protected:
	
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	void SetSide(InvoiceHentSide& side,const InvoiceHent& hent);
	void SetReadOnly(InvoiceHentSide& side,BOOL bReadOnly);
		
private:
	InvoiceHent m_hentToUpdate;
	InvoiceHent m_reference;	

	InvoiceHentSide m_invoiceSide;
	InvoiceHentSide m_referenceSide;

	afx_msg void OnUpdateName();
    afx_msg void OnUpdateStreetPOBox();
    afx_msg void OnUpdateCityZip();
    afx_msg void OnUpdateHentNo();
    afx_msg void OnUpdateFiscalNo();
    afx_msg void OnUpdateAccountNo();
    afx_msg void OnUpdateBank();
    afx_msg void OnUpdatePersonalNo();
    afx_msg void OnUpdatePersonalIdNo();
    afx_msg void OnUpdatePersonalIdIssueDt();
    afx_msg void OnUpdatePersonalIdIssuePost();
	afx_msg void OnNameChange();
	afx_msg void OnStreetChange();
	afx_msg void OnPOBoxChange();
	afx_msg void OnCityChange();
	afx_msg void OnZipChange();
	afx_msg void OnHentNoChange();
	afx_msg void OnFiscalNoChange();
	afx_msg void OnAccountNoChange();
	afx_msg void OnBankChange();
	afx_msg void OnPersonalNoChange();
	afx_msg void OnPersonalIdNoChange();
	afx_msg void OnPersonalIdIssueDtChange();
	afx_msg void OnPersonalIdIssuePostChange();



	DECLARE_MESSAGE_MAP();
};

#endif