#include "stdafx.h"
#include "BuyInvoiceCowsTransaction.h"
#include <data/runtime/InvoiceCowsScopeOperations.h>
#include <data/runtime/InvoiceTransactionItemTuple.h>
#include "TransactionException.h"
#include <errortoken/ErrorToken.h>
#include <boost/foreach.hpp>
#include <CommonTypes.h>
#include <data/runtime/DeductionItemTuple.h>

BuyInvoiceCowsTransaction::BuyInvoiceCowsTransaction(ISeasonInterface* pInterface, uint32_t transactionId) :
InvoiceTransactionSkeleton(pInterface, transactionId),
m_InDocId(NULL_ID)
{
}

boost::any BuyInvoiceCowsTransaction::ExecuteOperation(uint16_t operation, const OperationArguments& args)
{
	switch (operation)
	{
	case InvoiceCowsScopeOperations::Operation_SetHent:
		SetHent(args.get<InvoiceHent>(0));
		break;
	case InvoiceCowsScopeOperations::Operation_GetHent:
		return m_hent;
	case InvoiceCowsScopeOperations::Operation_GetInvoiceId:
		return m_invoiceId;

	case InvoiceCowsScopeOperations::Operation_GetInvoiceNo:
		return m_invoiceNo;


	case InvoiceCowsScopeOperations::Operation_GetInvoiceDetails:
		return m_details;
	case InvoiceCowsScopeOperations::Operation_SetInvoiceDetails:
		m_details.CopyFrom(args.get<InvoiceDetails>(0));
		break;
	case InvoiceCowsScopeOperations::Operation_UpdateCow:
	{
		ItemId id = args.get<ItemId>(0);
		CowDetails details = args.get<CowDetails>(1);
		break;
	}

	case InvoiceCowsScopeOperations::Operation_AddCow:
	{
		uint32_t cowId = args.get<uint32_t>(0);
		return AddCow(cowId);
	}

	case InvoiceCowsScopeOperations::Operation_UpdateCowInvoiceEntry:
	{
		ItemId id = args.get<ItemId>(0);
		CowInvoiceEntryDetails details = args.get<CowInvoiceEntryDetails>(1);
		InvoiceTransactionCowItem* item = AcquireItem(id);
		item->SetEntry(details);
		break;
	}
	case InvoiceCowsScopeOperations::Operation_GetItemsCount:
	{
		count_t itemCount = m_Items.size();
		return itemCount;
	}
	case InvoiceCowsScopeOperations::Operation_GetItemAt:
	{
		int index = args.get<int>(0);
		InvoiceTransactionCowItem* item = m_Items[index];
		InvoiceTransactionItemTuple itemTuple(item->GetItemKey(), item->GetCowId(), item->GetCow(), item->GetEntry());
		return itemTuple;
	}

	case InvoiceCowsScopeOperations::Operation_GetItem:
	{
		ItemId id = args.get<ItemId>(0);
		InvoiceTransactionCowItem* item = AcquireItem(id);
		InvoiceTransactionItemTuple itemTuple(item->GetItemKey(), item->GetCowId(), item->GetCow(), item->GetEntry());
		return itemTuple;
	}
	case InvoiceCowsScopeOperations::Operation_GetDoc:
		return m_InDocId;
	case InvoiceCowsScopeOperations::Operation_SetDoc:
	{
		SetDoc(args.get<uint32_t>(0));
		break;
	}
	case InvoiceCowsScopeOperations::Operation_DeleteItem:
	{
		ItemId id = args.get<ItemId>(0);
		DeleteItem(id);
		break;
	}

	case InvoiceCowsScopeOperations::Operation_GetDeductionsCount:
	{
		 return (count_t)m_Deductions.size();
	}
	case InvoiceCowsScopeOperations::Operation_GetDeductionAt:
	{
		int index = args.get<int>(0);
		DeductionTransactionItem* deduction = m_Deductions.item_at(index);
		return DeductionItemTuple(deduction->GetId(), deduction->GetDeduction());
	}

	case InvoiceCowsScopeOperations::Operation_GetDeduction:
	{
		ItemId id = args.get<ItemId>(0);
		DeductionTransactionItem* deduction = m_Deductions.find_item(id);
		return DeductionItemTuple(deduction->GetId(), deduction->GetDeduction());
	}
	case InvoiceCowsScopeOperations::Operation_AddDeduction:
	{
		return AddDeduction(args.get<InvoiceDeduction>(0));
	}
	case InvoiceCowsScopeOperations::Operation_RemoveDeduction:
	{
		DeleteDeduction(args.get<ItemId>(0));
		break;
	}

	case InvoiceCowsScopeOperations::Operation_UpdateDeduction:
	{
		DeductionItemTuple deductionTuple = args.get<DeductionItemTuple>(0);
		ItemId deductionItemId = deductionTuple.get<0>();
		DeductionTransactionItem* deductionItem = m_Deductions.find_item(deductionItemId);
		if (!deductionItem)
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"deduction_item_not_found")(L"id", ItemIdText::ToString(deductionItemId)).str()));

		InvoiceDeduction deduction = deductionTuple.get<1>();
		deductionItem->SetDeduction(deduction);
		break;																
	}

	case InvoiceCowsScopeOperations::Operation_GetInvoiceType:
		return m_type;
		
	case InvoiceCowsScopeOperations::Operation_SetInvoiceType:
		m_type = args.get<InvoiceType>(0);
		break;

	}
	return boost::any();
}

void BuyInvoiceCowsTransaction::SetDoc(uint32_t docId)
{
	if (m_InDocId != NULL_ID)
		BOOST_THROW_EXCEPTION(TransactionException(L"indoc already set for this transaction"));

	Do([docId](SeasonSessionFacade* facade)
	{
 		facade->LockInDoc(docId);
	});

	m_InDocId = docId;
}

void BuyInvoiceCowsTransaction::CheckIfDocIsSet()
{
	if (m_InDocId == NULL_ID)
		BOOST_THROW_EXCEPTION(TransactionException(L"indoc id not set for this invoice transaction"));
}

void BuyInvoiceCowsTransaction::DeleteItem(const ItemId& itemId)
{
	InvoiceTransactionCowItem* item = AcquireItem(itemId);

	uint32_t cowId = item->GetCowId();
	if (m_newlyAddedCows.find(cowId) != m_newlyAddedCows.end())
	{
		Do([cowId](SeasonSessionFacade* facade)
		{
			facade->UnlockCow(cowId);			
		});
		m_newlyAddedCows.erase(cowId);
	}
	InvoiceTransactionSkeleton::DeleteItem(itemId);
}

ItemId BuyInvoiceCowsTransaction::AddCow(uint32_t cowId)
{
	CheckIfDocIsSet();

	return Eval<ItemId>([this, cowId](SeasonSessionFacade* facade) -> ItemId
	{
		ICow* pCow = facade->FindCow(cowId);

		if(pCow == NULL)
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cow_not_found")(L"cow_id", cowId).str()));

		facade->LockCow(cowId);

		m_newlyAddedCows.insert(cowId);

		if (pCow->GetBuyInvoiceId() != NULL_ID)
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cow_already_has_a_buyinvoice")(L"invoice_id", pCow->GetBuyInvoiceId()).str()));

		ICowTrail* pTrail = facade->GetCowTrail(cowId);

		IDoc* pEntryDoc = pTrail->GetEntry()->GetDoc();

		if (pEntryDoc->GetDocType() != DocType_In)
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cow_not_entered_via_indoc").str()));

		if (pEntryDoc->GetId() != m_InDocId)
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cow_entered_via_other_indoc")(L"other_indoc_id", pEntryDoc->GetId()).str()));

		ItemId id = ObtainNewItemId();
		InvoiceTransactionCowItem* item = new InvoiceTransactionCowItem(InvoiceItemKey(DocKey(DocType_In, m_InDocId), id), cowId);
		CowDetails cowDetails;
		cowDetails.CopyFrom(pCow);
		item->SetCow(cowDetails);
		m_Items.push_back(item);

		return id;
	});
}

void BuyInvoiceCowsTransaction::New()
{
	m_invoiceId = NULL_ID;
	m_InDocId = NULL_ID;

	UserDo([this](UserSessionFacade* facade)
	{
		PtrEnumerator<IDeductionDefinition> enumDeductions = facade->EnumDeductionDefinitions();

		while (enumDeductions.hasNext())
		{
			IDeductionDefinition* definition = *enumDeductions;
			InvoiceDeduction deduction(definition);
			AddDeduction(deduction);
			enumDeductions.advance();
		}
	});

}

void BuyInvoiceCowsTransaction::Edit(uint32_t invoiceId)
{
	m_invoiceId = invoiceId;
	Do([this](SeasonSessionFacade* facade)
	{
		IBuyInvoice* pInvoice = facade->FindBuyInvoice(m_invoiceId);

		m_invoiceNo = pInvoice->GetInvoiceNo();
		m_type = pInvoice->GetInvoiceType();
		ICowInvoiceEntry* pEntry = pInvoice->GetCowAt(0);
		ICowTrail* pTrail = facade->GetCowTrail(pEntry->GetCowId());
		const ICowFootprint* pEntryFootprint = pTrail->GetEntry();
		IDoc* pEntryDoc = pEntryFootprint->GetDoc();
		m_InDocId = pEntryDoc->GetId();

		m_hent.CopyFrom(pInvoice->GetInvoiceHent());
		m_details.CopyFrom(pInvoice);

		ICow* pCow;
		uint32_t cowId;
		PtrEnumerator<ICowInvoiceEntry> entryEnum = pInvoice->EnumEntries();
		while (entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			cowId = pEntry->GetCowId();
			pCow = facade->FindCow(cowId);
			pTrail = facade->GetCowTrail(cowId);
			pEntryFootprint = pTrail->GetEntry();
			pEntryDoc = pEntryFootprint->GetDoc();

			InvoiceTransactionCowItem* item = new InvoiceTransactionCowItem(InvoiceItemKey(DocKey(pEntryDoc->GetDocType(), pEntryDoc->GetId()), ObtainNewItemId()), cowId);
			CowDetails cowDetails;
			cowDetails.CopyFrom(pCow);
			item->SetCow(cowDetails);

			CowInvoiceEntryDetails entryDetails;
			entryDetails.CopyFrom(pEntry);
			item->SetEntry(entryDetails);

			m_Items.push_back(item);
			entryEnum.advance();
		}

		IInvoiceDeduction* pDeduction;
		PtrEnumerator<IInvoiceDeduction> deductionsEnum = pInvoice->EnumDeductions();
		while (deductionsEnum.hasNext())
		{
			pDeduction = *deductionsEnum;
			AddDeduction(pDeduction);	
			deductionsEnum.advance();
		}

	});
}

uint32_t BuyInvoiceCowsTransaction::GetId() const
{
	return GetTransactionId();
}

void BuyInvoiceCowsTransaction::Commit()
{
	Do([this](SeasonSessionFacade* facade)
	{
		InvoiceSnapshot snapshot(m_invoiceId);
		snapshot.SetDetails(m_details);
		snapshot.SetInvoiceHent(m_hent);
		snapshot.SetInvoiceType(m_type);

		BOOST_FOREACH(InvoiceTransactionCowItem* item, m_Items)
		{
			InvoiceCowItem snaphotItem(item->GetCowId(), item->GetItemKey().GetDocKey().GetDocNo());
			snaphotItem.SetCow(item->GetCow());
			snaphotItem.SetEntry(item->GetEntry());
			snapshot.AddItem(snaphotItem);
		}

		BOOST_FOREACH(DeductionTransactionItem* deductionItem, m_Deductions)
		{
			snapshot.AddDeduction(deductionItem->GetDeduction());
		}

		if (m_invoiceId == NULL_ID)
		{
			facade->InsertBuyCowsInvoice(snapshot);
			facade->UnlockInDoc(m_InDocId);
		}
		else
		{
			facade->UpdateBuyCowsInvoice(snapshot);
		}

		BOOST_FOREACH(uint32_t cowId, m_newlyAddedCows)
		{
			facade->UnlockCow(cowId);
		}

	});
	Cleanup();
}

void BuyInvoiceCowsTransaction::Abort()
{
		Do([this](SeasonSessionFacade* facade)
		{

			if (m_invoiceId == NULL_ID)
			{
				facade->UnlockInDoc(m_InDocId); //shallow unlock
			}
			
			BOOST_FOREACH(uint32_t cowId, m_newlyAddedCows)
			{
					facade->UnlockCow(cowId);				
			}
		});

	Cleanup();
}


void BuyInvoiceCowsTransaction::Cleanup()
{
	InvoiceTransactionSkeleton::Cleanup();
	m_newlyAddedCows.clear();
}