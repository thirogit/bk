#include "stdafx.h"
#include "SellInvoiceDocScopeDlg.h"

#include "../utils/StringLoader.h"
#include <boost\functional\factory.hpp>

#include "../doc/DocSelectDlg.h"
#include "../header/HentsDocsListHeader.h"
#include "../header/SellInvoiceCalculationHeader.h"
#include <boost/foreach.hpp>
#include "../ui/ExceptionMsgBox.h"
#include "view/SellCalculationViewCreator.h"
#include "SellDocSelectInterface.h"
#include <string/TextUtils.h>
#include "../utils/StringLoader.h"
#include "../text/InvoiceTypeString.h"

BEGIN_MESSAGE_MAP(CSellInvoiceDocScopeDlg,CSellInvoiceDlg)
	
	ON_WM_DESTROY()

END_MESSAGE_MAP()

CSellInvoiceDocScopeDlg::CSellInvoiceDocScopeDlg(InvoiceDocsTransactionPtr transaction,CWnd* pParent/* = NULL*/) : 
	m_transaction(transaction),m_pDocsListContent(NULL),CSellInvoiceDlg(pParent)
{
}

CSellInvoiceDocScopeDlg::~CSellInvoiceDocScopeDlg()
{

}

void CSellInvoiceDocScopeDlg::SetupControls()
{
	m_itemlist.SetHeaderDescriptor(SellInvoiceCalculationHeader());

	SeasonSession* session = GetSeasonSession();

	m_pDocsListContent = new SimpleHentsDocsListContent(session);
	m_doclist.SetHeaderDescriptor(HentsDocsListHeader());
	m_doclist.SetContent(m_pDocsListContent);

	
	m_pCalculation = new InvoiceCalculation(m_transaction->GetFacade());

	if(m_transaction->IsUpdating())
	{		
		m_pCalculation->Edit();
		SetWindowText(TextUtils::Format(StringLoader(IDS_EDITSELLINVOICEDLGCAPTION), InvoiceTypeString(m_transaction->GetInvoiceType()).ToString(),m_transaction->GetInvoiceNo()).c_str());
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

	m_viewCreatorFactory = boost::bind(boost::factory<SellCalculationViewCreator*>(),_1);

	RefreshDocsList();
}

void CSellInvoiceDocScopeDlg::OnDestroy()
{
	m_doclist.SetContent(NULL);
	CSellInvoiceDlg::OnDestroy();
	delete m_pCalculation;
	delete m_pDocsListContent;
}

void CSellInvoiceDocScopeDlg::AddItem()
{
	SellDocKamilWSelectInterface selectInterface;
	selectInterface.Create(GetSeasonSession(),m_transaction);
	DocSelectDlg docSelectDlg(&selectInterface,this);
	docSelectDlg.SetHeader(HentsDocsListHeader());
	docSelectDlg.DoModal();
	selectInterface.Destroy();
	RefreshDocsList();
}

void CSellInvoiceDocScopeDlg::DelItem()
{
	int nSelectedRow = m_doclist.GetCurSel();
	if(nSelectedRow >= 0)
	{
		HentsDocRow* pRow = m_pDocsListContent->GetDocAtRow(nSelectedRow);
		ExceptionMsgBox().TryCatchAny([this,pRow]
		{
			m_transaction->DeleteDoc(pRow->Content()->GetDocId());
			m_itemlist.UnselectAll();
		});
		RefreshDocsList();
	}
}

void CSellInvoiceDocScopeDlg::RefreshDocsList()
{
	SeasonSession* session = GetSeasonSession();
	m_pDocsListContent->RemoveAllDocs();
	
	uint32_t docId;
						
	for(int i = 0,count = m_transaction->GetDocCount();i < count;i++)
	{
		docId = m_transaction->GetDocAt(i);
		SellDocClientObject* pSellDocCO = session->GetSellDoc(docId);							
		m_pDocsListContent->AddDoc(pSellDocCO->Object());
	}			
	
}

void CSellInvoiceDocScopeDlg::SaveInvoice()
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

std::wstring CSellInvoiceDocScopeDlg::GetCaption()
{
	if (m_transaction->IsUpdating())
	{
		return TextUtils::Format(StringLoader(IDS_EDITSELLINVOICECAPTION), m_transaction->GetInvoiceNo());
	}
	else
	{
		return StringLoader(IDS_NEWSELLINVOICECAPTION);
	}

	
}