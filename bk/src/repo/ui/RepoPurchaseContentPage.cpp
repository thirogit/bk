#include "stdafx.h"
#include "RepoPurchaseContentPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRepoPurchaseContentPage, ETSLayoutDialog)
CRepoPurchaseContentPage::CRepoPurchaseContentPage()
	: ETSLayoutDialog(CRepoPurchaseContentPage::IDD,NULL,NULL,false)
{	
}

CRepoPurchaseContentPage::~CRepoPurchaseContentPage()
{
	
}

void CRepoPurchaseContentPage::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PURACHE_INVOICE_LIST,m_InvoiceList);
	DDX_Control(pDX,IDC_PURACHE_INVOICE_COW_LIST,m_CowList);
	DDX_Control(pDX,IDC_PURACHE_INVOICE_DEDUCTIONS_LIST, m_DeductionsList);	
}


BEGIN_MESSAGE_MAP(CRepoPurchaseContentPage, ETSLayoutDialog)
	ON_NOTIFY(NM_CLICK,	IDC_PURACHE_INVOICE_LIST, OnInvoiceClick)
	ON_WM_ENABLE()
END_MESSAGE_MAP()


BOOL CRepoPurchaseContentPage::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();	
	
	CPane rootPane = CreateRoot(VERTICAL,0);
	rootPane->addItem(&m_InvoiceList);
	rootPane->addItem(&m_CowList);
	rootPane->addItem(&m_DeductionsList);	
		
	return TRUE;
}


void CRepoPurchaseContentPage::UpdateFrom(IPurchase* pPurchase)
{
	m_pPurchase = pPurchase;
	m_InvoiceListContent.Clear();
	m_CowListContent.Clear();
	m_DeductionListContent.Clear();

	m_InvoiceList.EnableWindow(pPurchase != NULL);	
	m_CowList.EnableWindow(FALSE);
	m_DeductionsList.EnableWindow(FALSE);

	if (pPurchase != NULL)
	{
		m_InvoiceList.SetContent(NULL);
		PtrEnumerator<IPurchaseInvoice> enumInvoices = pPurchase->EnumInvoices();
		while (enumInvoices.hasNext())
		{
			m_InvoiceListContent.AddRow(*enumInvoices);
			enumInvoices.advance();
		}
		m_InvoiceList.SetContent(&m_InvoiceListContent);

		if (pPurchase->GetInvoiceCount() == 1)
		{
			m_CowList.EnableWindow(TRUE);
			m_DeductionsList.EnableWindow(TRUE);

			IPurchaseInvoice* invoice = pPurchase->GetInvoiceAt(0);
			UpdateCowListFrom(invoice);			
			UpdateDeductionListFrom(invoice);
		}
		
		
	}
}

void CRepoPurchaseContentPage::UpdateCowListFrom(IPurchaseInvoice* invoice)
{
	m_CowList.SetContent(NULL);
	m_CowListContent.Clear();

	if (invoice != NULL)
	{
		PtrEnumerator<IPurchaseCow> enumCows = invoice->EnumCows();
		while (enumCows.hasNext())
		{
			m_CowListContent.AddRow(*enumCows);
			enumCows.advance();
		}

		m_CowList.SetContent(&m_CowListContent);
	}
}

void CRepoPurchaseContentPage::UpdateDeductionListFrom(IPurchaseInvoice* invoice)
{
	m_DeductionsList.SetContent(NULL);
	m_DeductionListContent.Clear();

	if (invoice != NULL)
	{
		PtrEnumerator<IPurchaseInvoiceDeduction> enumDeducions = invoice->EnumDeductions();
		while (enumDeducions.hasNext())
		{
			m_DeductionListContent.AddRow(*enumDeducions);
			enumDeducions.advance();
		}
		m_DeductionsList.SetContent(&m_DeductionListContent);
	}
}

void CRepoPurchaseContentPage::OnInvoiceClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE plvItemActive = (LPNMITEMACTIVATE)pNMHDR;
    
	if(plvItemActive->iItem >= 0)
	{
		PurchaseInvoiceRow* invoiceRow = m_InvoiceListContent.GetInvoiceRowAt(plvItemActive->iItem);
		PurchaseInvoiceView* invoiceView = invoiceRow->Content();
		m_CowList.EnableWindow(TRUE);
		m_DeductionsList.EnableWindow(TRUE);
		IPurchaseInvoice* invoice = m_pPurchase->GetInvoice(invoiceView->GetInvoiceId());
		UpdateCowListFrom(invoice);
		UpdateDeductionListFrom(invoice);
	}
	*pResult = 0;
}

void CRepoPurchaseContentPage::OnEnable(BOOL bEnable)
{
	m_InvoiceList.EnableWindow(bEnable);
	m_CowList.EnableWindow(bEnable);
}