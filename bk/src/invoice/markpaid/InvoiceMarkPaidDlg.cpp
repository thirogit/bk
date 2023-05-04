#include "stdafx.h"
#include "InvoiceMarkPaidDlg.h"
#include <boost/foreach.hpp>
#include "../../ui/askfordatedlg/AskForDateDlg.h"
#include <utils/SafeGet.h>
#include "../../header/InvoiceListHeader.h"
#include "../../resource.h"
#include "../../utils/StringLoader.h"

BEGIN_MESSAGE_MAP(CInvoiceMarkPaidDlg, CSessionAwareDialog)
	ON_BN_CLICKED(IDC_MARKPAIDNOW, OnMarkPaidNow)
	ON_BN_CLICKED(IDC_MARKPAIDDATE, OnMarkPaidDate)
	ON_BN_CLICKED(IDC_MARKPAIDNO, OnMarkPaidNo)

	ON_BN_CLICKED(IDC_SELECTALL, OnSelectAll)
	ON_BN_CLICKED(IDC_INVERSELECTION, OnInvertSelection)
	ON_BN_CLICKED(IDC_SELECTNONE, OnSelectNone)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


CInvoiceMarkPaidDlg::CInvoiceMarkPaidDlg(CWnd* pParent) : CSessionAwareDialog(IDD,pParent)
{
	m_pContent = NULL;
}

CInvoiceMarkPaidDlg::~CInvoiceMarkPaidDlg()
{
}

void CInvoiceMarkPaidDlg::OnDestroy()
{
	CSessionAwareDialog::OnDestroy();
	m_invoiceList.SetContent(NULL);
	delete m_pContent;	
}

BOOL CInvoiceMarkPaidDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();


	m_invoiceList.SetHeaderDescriptor(InvoiceListHeader());
	m_pContent = new SimpleInvoicesListContent(GetSeasonSession());
	m_invoiceList.SetContent(m_pContent);
	
	return TRUE;
}

void CInvoiceMarkPaidDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_INVOICELIST,m_invoiceList);
}

void CInvoiceMarkPaidDlg::OnOK()
{
	EndDialog(IDOK);
}

void CInvoiceMarkPaidDlg::MarkSelectedInvoices(const DateTime& paidDt)
{
	ListSelection selection = m_invoiceList.GetSelection();
	InvoiceRow* row;
	InvoiceView* view;
	BOOST_FOREACH(int nrow,selection.GetRange())
	{
		row = m_pContent->GetInvoiceRowAt(nrow);
		view = row->Content();
		MarkInvoice(view->GetInvoiceId(),paidDt);
	}
}

void CInvoiceMarkPaidDlg::AddInvoice(IInvoice* pInvoice)
{
	m_pContent->AddRow(pInvoice);
}

void CInvoiceMarkPaidDlg::RemoveInvoice(uint32_t invoiceId)
{
	m_pContent->RemoveRow(invoiceId);
}

const InvoiceView* CInvoiceMarkPaidDlg::GetInvoice(uint32_t invoiceId) const
{
	return SafeGet(&InvoiceRow::Content,m_pContent->FindRow(invoiceId),nullptr);
}

void CInvoiceMarkPaidDlg::UpdateInvoice(IInvoice* pInvoice)
{
	m_pContent->UpdateRow(pInvoice);
}

void CInvoiceMarkPaidDlg::OnMarkPaidNow()
{
	MarkSelectedInvoices(DateTime::now());
}

void CInvoiceMarkPaidDlg::OnMarkPaidDate()
{
	CAskForDateDlg askForDt(this);
	askForDt.SetWindowCaption(StringLoader(IDS_ASKENTERPAIDDT));
	askForDt.SetLabel(StringLoader(IDS_PAIDDATELBL));
	DateTime paidDt = askForDt.AskForDate();
	if(!paidDt.IsNull())
		MarkSelectedInvoices(paidDt);

}

void CInvoiceMarkPaidDlg::OnMarkPaidNo()
{
	MarkSelectedInvoices(DateTime());
}

void CInvoiceMarkPaidDlg::OnSelectAll()
{
	m_invoiceList.SelectAll();
}

void CInvoiceMarkPaidDlg::OnInvertSelection()
{
	m_invoiceList.InvertSelection();
}

void CInvoiceMarkPaidDlg::OnSelectNone()
{
	m_invoiceList.SelectNone();
}