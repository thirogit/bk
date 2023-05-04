#include "stdafx.h"
#include "BuyInvoiceCowScopeDlg.h"
#include "../header/BuyInvoiceCalculationHeader.h"
#include "../header/OwnDocsListHeader.h"
#include "view/BuyCalculationViewCreator.h"
#include <boost\functional\factory.hpp>
#include <boost/foreach.hpp>
#include "BuyInvoiceCowsSelectInterface.h"
#include "../cow/CowSelectDlg.h"
#include "../ui/ExceptionMsgBox.h"
#include <string/TextUtils.h>
#include "../utils/StringLoader.h"
#include "../text/InvoiceTypeString.h"

BEGIN_MESSAGE_MAP(CBuyInvoiceCowScopeDlg,CBuyInvoiceDlg)
	
	ON_WM_DESTROY()

END_MESSAGE_MAP()

CBuyInvoiceCowScopeDlg::CBuyInvoiceCowScopeDlg(InvoiceCowsTransactionPtr transaction,CWnd* pParent/* = NULL*/) : 
	m_transaction(transaction),m_pDocsListContent(NULL),CBuyInvoiceDlg(pParent)
{
}

CBuyInvoiceCowScopeDlg::~CBuyInvoiceCowScopeDlg()
{

}

void CBuyInvoiceCowScopeDlg::SetupControls()
{
	m_itemlist.SetHeaderDescriptor(BuyInvoiceCalculationHeader());

	SeasonSession* session = GetSeasonSession();

	m_pDocsListContent = new SimpleInDocsListContent(session);
	m_doclist.SetHeaderDescriptor(OwnDocsListHeader());
	m_doclist.SetContent(m_pDocsListContent);

	m_pCalculation = new InvoiceCalculation(m_transaction->GetFacade());

	if (m_transaction->IsUpdating())
	{
		m_pCalculation->Edit();
		SetWindowText(TextUtils::Format(StringLoader(IDS_EDITBUYINVOICEDLGCAPTION), InvoiceTypeString(m_transaction->GetInvoiceType()).ToString(), m_transaction->GetInvoiceNo()).c_str());
	}
	else
	{
		m_pCalculation->New();
		SetWindowText(TextUtils::Format(StringLoader(IDS_NEWBUYINVOICEDLGCAPTION), InvoiceTypeString(m_transaction->GetInvoiceType()).ToString()).c_str());
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
	InDocClientObject* pDocCO = session->GetInDoc(docId);
	m_pDocsListContent->AddDoc(pDocCO->Object());

	m_viewCreatorFactory = boost::bind(boost::factory<BuyCalculationViewCreator*>(),_1);
}

void CBuyInvoiceCowScopeDlg::OnDestroy()
{
	m_doclist.SetContent(NULL);
	CBuyInvoiceDlg::OnDestroy();
	delete m_pCalculation;
	delete m_pDocsListContent;
}

void CBuyInvoiceCowScopeDlg::AddItem()
{
	BuyInvoiceCowsSelectInterface selectInterface;
	selectInterface.Create(GetSeasonSession(),m_transaction->GetFacade());
	CowSelectDlg cowSelectDlg(&selectInterface,this);
	cowSelectDlg.SetHeader(OwnDocsListHeader());
	cowSelectDlg.DoModal();
	selectInterface.Destroy();
}

void CBuyInvoiceCowScopeDlg::DelItem()
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

void CBuyInvoiceCowScopeDlg::SaveInvoice()
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

std::wstring CBuyInvoiceCowScopeDlg::GetCaption()
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