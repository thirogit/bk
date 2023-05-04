#include "stdafx.h"
#include "CowPresenceViewCreator.h"
#include <utils/SafeGet.h>
#include "../text/DocNoText.h"
#include "../text/InvoiceNoText.h"

CowPresenceViewCreator::CowPresenceViewCreator(SeasonSession* facade) : m_pFacade(facade),m_hentViewCreator(facade)
{
}

void CowPresenceViewCreator::RefreshView(CowPresenceView* pView,ICowPresence* pHerdPresence)
{
	CowPresenceKey key = pView->GetKey();
	uint32_t cowId = key.GetCowId();
	CowClientObject* pCowCO = m_pFacade->GetCow(cowId);
	ICow* pCow = pCowCO->Object();
	ICowTrail* pTrail = m_pFacade->GetCowTrail(cowId);
	pView->SetLocked(pCowCO->IsLocked());	
	pView->SetInStock(pHerdPresence->IsInStock());	
	pView->SetCowNo(pCow->GetCowNo());
	pView->SetPassNo(pCow->GetPassNo());

	uint32_t firstOwnerId = pCow->GetFirstOwnerId();
	if(firstOwnerId != NULL_ID)
	{
		HentView firstOwnerView(firstOwnerId);
		m_hentViewCreator.RefreshView(&firstOwnerView,m_pFacade->GetHent(firstOwnerView.GetHentId()));
		pView->SetFirstOwner(&firstOwnerView);
	}
	else
	{
		pView->SetFirstOwner(NULL);
	}

	pView->SetBirthDate(pCow->GetBirthDate());
	pView->SetWeight(pCow->GetWeight());
	pView->SetSex(pCow->GetSex());

	IStock* pOwnStock = m_pFacade->GetStock(pCow->GetStockId());
	pView->SetOwnStockCd(SafeGet(&IStock::GetStockCode,pOwnStock,L""));
	pView->SetOwnStockId(pCow->GetStockId());

	IClass* pOwnClass = m_pFacade->GetClass(pCow->GetClassId());
	pView->SetOwnClassCd(SafeGet(&IClass::GetClassCode,pOwnClass,L""));
	pView->SetOwnClassId(pCow->GetClassId());

	pView->SetMotherNo(pCow->GetMotherNo());
	pView->SetHealthCertNo(pCow->GetHealthCertNo());
	pView->SetExtras(pCow->GetExtras());

	IClass* pTermBuyClass = m_pFacade->GetClass(pCow->GetTermBuyClassId());
	pView->SetTermBuyClassCd(SafeGet(&IClass::GetClassCode,pTermBuyClass,L""));

	IClass* pTermSellClass = m_pFacade->GetClass(pCow->GetTermSellClassId());
	pView->SetTermSellClassCd(SafeGet(&IClass::GetClassCode,pTermSellClass,L""));

	pView->SetTermBuyWeight(pCow->GetTermBuyWeight());
	pView->SetTermSellWeight(pCow->GetTermSellWeight());
	pView->SetTermBuyPrice(pCow->GetTermBuyPrice());
	pView->SetTermSellPrice(pCow->GetTermSellPrice());

	const ICowFootprint* pEntry = pTrail->GetEntry();
	IDoc* pEntryDoc = pEntry->GetDoc();

	pView->SetHerdInPlateNo(pEntryDoc->GetPlateNo());

	uint32_t buyInvoiceId = NULL_ID;

	switch(pEntryDoc->GetDocType())
	{
	case DocType_Buy:
		{
			IBuyDoc* pBuyEntryDoc = (IBuyDoc*)pEntryDoc;
			buyInvoiceId = pBuyEntryDoc->GetInvoiceId();	
						
			HentView buyDocHentView(pBuyEntryDoc->GetHentId());
			m_hentViewCreator.RefreshView(&buyDocHentView, m_pFacade->GetHent(pBuyEntryDoc->GetHentId()));
			pView->SetBuyDocHent(&buyDocHentView);
			
			break;
		}
	case DocType_In:
		{
			 buyInvoiceId = pCow->GetBuyInvoiceId();						
			break;
		}
	}

	if(buyInvoiceId != NULL_ID)
	{
		BuyInvoiceClientObject* pBuyInvoiceCO = m_pFacade->GetBuyInvoice(buyInvoiceId);
		BuyInvoice* pBuyInvoice = pBuyInvoiceCO->Object();

		const IInvoiceHent* pHent = pBuyInvoice->GetInvoiceHent();
		HentView invoiceHentView(pHent->GetParentHentId());
		m_hentViewCreator.RefreshView(&invoiceHentView,m_pFacade->GetHent(invoiceHentView.GetHentId()));
		pView->SetBuyFromHent(&invoiceHentView);

		ICowInvoiceEntry* pBuyEntry = pBuyInvoice->GetCow(cowId);
		IClass* pBuyClass = m_pFacade->GetClass(pBuyEntry->GetClassId());
		pView->SetBuyClassCd(SafeGet(&IClass::GetClassCode,pBuyClass,L""));
		pView->SetBuyPrice(NullDecimal(pBuyEntry->GetPrice()));
		pView->SetBuyWeight(NullDecimal(pBuyEntry->GetWeight()));
		pView->SetBuyInvoiceNo(InvoiceNoText(pBuyInvoice).ToString());
	}
	else
	{
		pView->SetBuyFromHent(NULL);		
		pView->SetBuyClassCd(L"");
		pView->SetBuyPrice(NullDecimal());
		pView->SetBuyWeight(NullDecimal());
		pView->SetBuyInvoiceNo(L"");
	}
		
	
	pView->SetHerdInDocNo(DocNoText(pHerdPresence->GetHerdInDoc()).ToString());				
	pView->SetHerdInDate(pHerdPresence->GetHerdInDate());
	pView->SetHerdInGrp(pHerdPresence->GetHerdInGrp());
	IClass* pHerdInClass = m_pFacade->GetClass(pHerdPresence->GetHerdInClassId());
	pView->SetHerdInClassCd(SafeGet(&IClass::GetClassCode,pHerdInClass,L""));				
	pView->SetHerdInWeight(pHerdPresence->GetHerdInWeight());
	pView->SetHerdInMotive(pEntryDoc->GetMotive());

	const ICowFootprint* pExit = pTrail->GetExit();
				
	if(pExit != NULL)
	{
		IDoc* pExitDoc = pExit->GetDoc();

		pView->SetHerdOutPlateNo(pExitDoc->GetPlateNo());
		pView->SetHerdOutMotive(pExitDoc->GetMotive());

		uint32_t sellInvoiceId = NULL_ID;

		switch(pExitDoc->GetDocType())
		{
			case DocType_Sell:
			{
				ISellDoc* pExitSellDoc = (ISellDoc*)pExitDoc;
				sellInvoiceId = pExitSellDoc->GetInvoiceId();

				HentView sellDocHentView(pExitSellDoc->GetHentId());
				m_hentViewCreator.RefreshView(&sellDocHentView, m_pFacade->GetHent(sellDocHentView.GetHentId()));
				pView->SetSellDocHent(&sellDocHentView);

				break;
			}
			case DocType_Out:
			{
				sellInvoiceId = pCow->GetSellInvoiceId();				
				break;
			}
		}

		if(sellInvoiceId != NULL_ID)
		{
			SellInvoiceClientObject* pSellInvoiceCO = m_pFacade->GetSellInvoice(sellInvoiceId);
			SellInvoice* pSellInvoice = pSellInvoiceCO->Object();

			const IInvoiceHent* pHent = pSellInvoice->GetInvoiceHent();
			HentView invoiceHentView(pHent->GetParentHentId());
			m_hentViewCreator.RefreshView(&invoiceHentView,m_pFacade->GetHent(invoiceHentView.GetHentId()));
			pView->SetSellToHent(&invoiceHentView);

			ICowInvoiceEntry* pSellEntry = pSellInvoice->GetCow(cowId);
			IClass* pSellClass = m_pFacade->GetClass(pSellEntry->GetClassId());
			pView->SetSellClassCd(SafeGet(&IClass::GetClassCode,pSellClass,L""));
			pView->SetSellPrice(NullDecimal(pSellEntry->GetPrice()));
			pView->SetSellWeight(NullDecimal(pSellEntry->GetWeight()));
			pView->SetSellInvoiceNo(InvoiceNoText(pSellInvoice).ToString());
		}
		else
		{
			pView->SetSellToHent(NULL);			
			pView->SetSellClassCd(L"");
			pView->SetSellPrice(NullDecimal());
			pView->SetSellWeight(NullDecimal());
			pView->SetSellInvoiceNo(L"");
		}
	}

	if(!pHerdPresence->IsInStock())
	{
		pView->SetHerdOutDocNo(DocNoText(pHerdPresence->GetHerdOutDoc()).ToString());
		pView->SetHerdOutDate(pHerdPresence->GetHerdOutDate());
		pView->SetHerdOutGrp(pHerdPresence->GetHerdOutGrp());

		IClass* pHerdOutClass = m_pFacade->GetClass(pHerdPresence->GetHerdOutClassId());
		pView->SetHerdOutClassCd(SafeGet(&IClass::GetClassCode,pHerdOutClass,L""));
		pView->SetHerdOutWeight(pHerdPresence->GetHerdOutWeight());
	}

	//pView->SetInvoiceNo(const std::string& sInvoiceNo);

}

CowPresenceView* CowPresenceViewCreator::CreateView(ICowPresence* pHerdPresence)
{
	uint32_t cowId = pHerdPresence->GetCowId();
	const IDoc* pEnteringDoc = pHerdPresence->GetHerdInDoc();
	CowPresenceView* pView = new CowPresenceView(CowPresenceKey(cowId,pEnteringDoc->GetDocType(),pEnteringDoc->GetId()));
	RefreshView(pView,pHerdPresence);
	return pView;			
}


