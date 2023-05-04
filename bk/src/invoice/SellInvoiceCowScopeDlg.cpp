#include "stdafx.h"
#include "SellInvoiceCowScopeDlg.h"
#include "../header/SellInvoiceCalculationHeader.h"
#include "../header/OwnDocsListHeader.h"
#include "view/SellCalculationViewCreator.h"
#include <boost\functional\factory.hpp>
#include <boost/foreach.hpp>
#include "SellInvoiceCowsSelectInterface.h"
#include "../cow/CowSelectDlg.h"
#include "../ui/ExceptionMsgBox.h"
#include <string\TextUtils.h>
#include "../utils/StringLoader.h"
#include "../text/InvoiceTypeString.h"

BEGIN_MESSAGE_MAP(CSellInvoiceCowScopeDlg,CSellInvoiceDlg)
	
	ON_WM_DESTROY()

END_MESSAGE_MAP()

CSellInvoiceCowScopeDlg::CSellInvoiceCowScopeDlg(InvoiceCowsTransactionPtr transaction,CWnd* pParent/* = NULL*/) : 
	m_transaction(transaction),m_pDocsListContent(NULL),CSellInvoiceDlg(pParent)
{
}

CSellInvoiceCowScopeDlg::~CSellInvoiceCowScopeDlg()
{

}

void CSellInvoiceCowScopeDlg::SetupControls()
{
	m_itemlist.SetHeaderDescriptor(SellInvoiceCalculationHeader());

	SeasonSession* session = GetSeasonSession();

	m_pDocsListContent = new SimpleInDocsListContent(session);
	m_doclist.SetHeaderDescriptor(OwnDocsListHeader());
	m_doclist.SetContent(m_pDocsListContent);

	m_pCalculation = new InvoiceCalculation(m_transaction->GetFacade());

	if(m_transaction->IsUpdating())
	{		
		m_pCalculation->Edit();			
		SetWindowText(TextUtils::Format(StringLoader(IDS_EDITSELLINVOICEDLGCAPTION), InvoiceTypeString(m_transaction->GetInvoiceType()).ToString(), m_transaction->GetInvoiceNo()).c_str());
	}
	else
	{
		m_pCalculation->New();		
		SetWindowText(TextUtils::Format(StringLoader(IDS_NEWSELLINVOICEDLGCAPTION), InvoiceTypeString(m_transaction->GetInvoiceType()).ToString()).c_str());
	}

	const InvoiceHent& hent = m_transaction->GetHent();
	const InvoiceDetails& details = m_transaction->GetDetails();
	m_pCalculation->SetVATRate(details.GetVATRate());
	SetDetails(details);

	uint32_t parentHentId = hent.GetParentHentId();
	if (parentHentId != NULL_ID)
	{
		m_hentbtn.InitHent(parentHentId);
		m_hent.CopyFrom(hent);
		m_hentgroup.EnableWindow(TRUE);
	}

	uint32_t docId = m_transaction->GetDocId();
	OutDocClientObject* pDocCO = session->GetOutDoc(docId);
	m_pDocsListContent->AddDoc(pDocCO->Object());

	m_viewCreatorFactory = boost::bind(boost::factory<SellCalculationViewCreator*>(),_1);
}

void CSellInvoiceCowScopeDlg::OnDestroy()
{
	m_doclist.SetContent(NULL);
	CSellInvoiceDlg::OnDestroy();
	delete m_pCalculation;
	delete m_pDocsListContent;
}

void CSellInvoiceCowScopeDlg::AddItem()
{
	SellInvoiceCowsSelectInterface selectInterface;
	selectInterface.Create(GetSeasonSession(),m_transaction->GetFacade());
	CowSelectDlg cowSelectDlg(&selectInterface,this);
	cowSelectDlg.SetHeader(OwnDocsListHeader());
	cowSelectDlg.DoModal();
	selectInterface.Destroy();
}

void CSellInvoiceCowScopeDlg::DelItem()
{
	int nSelectedRow = m_itemlist.GetCurSel();
	if(nSelectedRow >= 0)
	{
		CalculationElementRow* pRow = m_pContent->GetCalculationRow(nSelectedRow);
		ExceptionMsgBox().TryCatchAny([this,pRow]
		{
			m_transaction->DeleteItem(pRow->Content()->GetItemId());
			m_itemlist.UnselectAll();
		});
	}
}

void CSellInvoiceCowScopeDlg::SaveInvoice()
{
	BOOST_FOREACH(const CalculationElement* pElement,m_pCalculation->Elements())
	{
		CowInvoiceEntryDetails entryDetails;
		entryDetails.SetClassId(pElement->GetClassId());
		entryDetails.SetWeight(pElement->GetWeight());
		entryDetails.SetPrice(pElement->GetNetPrice());
		m_transaction->UpdateEntry(pElement->GetId(),entryDetails);
	}	
	m_transaction->UpdateDetails(GetDetails());
	m_transaction->SetHent(GetInvoiceHent());
	m_transaction->Commit();
}


std::wstring CSellInvoiceCowScopeDlg::GetCaption()
{
	if (m_transaction->IsUpdating())
	{
		return TextUtils::Format(StringLoader(IDS_EDITBUYINVOICECAPTION), m_transaction->GetInvoiceNo());
	}
	else
	{
		return StringLoader(IDS_NEWBUYINVOICECAPTION);
	}


}