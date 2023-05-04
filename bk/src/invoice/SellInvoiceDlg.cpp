#include "stdafx.h"
#include "SellInvoiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CSellInvoiceDlg, CBaseInvoiceDlg)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


CSellInvoiceDlg::CSellInvoiceDlg(CWnd* pParent /*=NULL*/) : CBaseInvoiceDlg(pParent)
{
	m_itemsHdrLayout = NULL;
	m_deductionsHdrLayout = NULL;
	m_docsHdrLayout = NULL;
}


CSellInvoiceDlg::~CSellInvoiceDlg()
{
	delete m_itemsHdrLayout;
	delete m_deductionsHdrLayout;
	delete m_docsHdrLayout;
}

BOOL CSellInvoiceDlg::OnInitDialog()
{		
	CBaseInvoiceDlg::OnInitDialog();
	
	SeasonSession* session = GetSeasonSession();

	m_itemsHdrLayout = new HeaderLayoutConfiguration(L"SELL_INVOICE_ITEMS");
	m_deductionsHdrLayout = new HeaderLayoutConfiguration(L"SELL_INVOICE_DEDUCTIONS");
	m_docsHdrLayout = new HeaderLayoutConfiguration(L"SELL_INVOICE_DOCS");

	m_itemsHdrLayout->Load(session);
	m_deductionsHdrLayout->Load(session);
	m_docsHdrLayout->Load(session);

	m_itemlist.SetHeaderLayout(m_itemsHdrLayout);
	m_doclist.SetHeaderLayout(m_docsHdrLayout);
	m_deductionslist.SetHeaderLayout(m_deductionsHdrLayout);

	return TRUE;
}

void CSellInvoiceDlg::OnDestroy()
{
	SeasonSession* session = GetSeasonSession();

	m_itemsHdrLayout->Save(session);
	m_deductionsHdrLayout->Save(session);
	m_docsHdrLayout->Save(session);

	CBaseInvoiceDlg::OnDestroy();

}

