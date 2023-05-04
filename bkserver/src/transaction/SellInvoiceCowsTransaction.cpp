#include "stdafx.h"
#include "SellInvoiceCowsTransaction.h"
#include <data/runtime/InvoiceCowsScopeOperations.h>
#include <data/runtime/InvoiceTransactionItemTuple.h>
#include "TransactionException.h"
#include <errortoken/ErrorToken.h>
#include <boost/foreach.hpp>
#include <CommonTypes.h>
#include <data/runtime/DeductionItemTuple.h>

SellInvoiceCowsTransaction::SellInvoiceCowsTransaction(ISeasonInterface* pInterface,uint32_t transactionId) : 
	InvoiceTransactionSkeleton(pInterface,transactionId),
	m_OutDocId(NULL_ID)
{
}

boost::any SellInvoiceCowsTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
{
	switch(operation)
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
				InvoiceTransactionItemTuple itemTuple(item->GetItemKey(),item->GetCowId(),item->GetCow(),item->GetEntry());
				return itemTuple;
			}	

		case InvoiceCowsScopeOperations::Operation_GetItem:		
		{
			ItemId id = args.get<ItemId>(0);
			InvoiceTransactionCowItem* item = AcquireItem(id);
			InvoiceTransactionItemTuple itemTuple(item->GetItemKey(),item->GetCowId(),item->GetCow(),item->GetEntry());
			return itemTuple;
		}	
		case InvoiceCowsScopeOperations::Operation_GetDoc:
			return m_OutDocId;
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

		case InvoiceCowsScopeOperations::Operation_GetInvoiceType:
			return m_type;

		case InvoiceCowsScopeOperations::Operation_SetInvoiceType:
			m_type = args.get<InvoiceType>(0);
			break;

	}
	return boost::any();
}

void SellInvoiceCowsTransaction::SetDoc(uint32_t docId)
{
	if(m_OutDocId != NULL_ID)
		BOOST_THROW_EXCEPTION(TransactionException(L"outdoc already set for this transaction"));

	Do([docId](SeasonSessionFacade* facade)
	{
		facade->LockOutDoc(docId);
	});

	m_OutDocId = docId;
}

void SellInvoiceCowsTransaction::CheckIfDocIsSet()
{
	if(m_OutDocId == NULL_ID)
		BOOST_THROW_EXCEPTION(TransactionException(L"outdoc id not set for this invoice transaction"));
}

ItemId SellInvoiceCowsTransaction::AddCow(uint32_t cowId)
{
	CheckIfDocIsSet();

	return Eval<ItemId>([this,cowId](SeasonSessionFacade* facade) -> ItemId
			{
				ICow* pCow = facade->FindCow(cowId);

				if(pCow->GetSellInvoiceId() != NULL_ID)
					BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cow_already_has_a_sellinvoice")(L"invoice_id",pCow->GetSellInvoiceId()).str()));

				ICowTrail* pTrail = facade->GetCowTrail(cowId);


				const ICowFootprint* pExitFootprint = pTrail->GetExit();
				if(!pExitFootprint)
					BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cow_not_exited_herd").str()));

				IDoc* pExitDoc = pExitFootprint->GetDoc();

				if(pExitDoc->GetDocType() != DocType_Out)
					BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cow_not_exited_via_outdoc").str()));

				if(pExitDoc->GetId() != m_OutDocId)
					BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cow_exited_via_other_outdoc")(L"other_indoc_id",pExitDoc->GetId()).str()));
								
				ItemId id = ObtainNewItemId();
				InvoiceTransactionCowItem* item = new InvoiceTransactionCowItem(InvoiceItemKey(DocKey(DocType_In,m_OutDocId),id),cowId);
				CowDetails cowDetails;
				cowDetails.CopyFrom(pCow);
				item->SetCow(cowDetails);
				m_Items.push_back(item);

				return id;
			});
}

void SellInvoiceCowsTransaction::New()
{
	m_invoiceId	= NULL_ID;
	m_OutDocId = NULL_ID;

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

void SellInvoiceCowsTransaction::Edit(uint32_t invoiceId)
{
	m_invoiceId = invoiceId;
	Do([this](SeasonSessionFacade* facade)
	{
		ISellInvoice* pInvoice = facade->FindSellInvoice(m_invoiceId);
		m_invoiceNo = pInvoice->GetInvoiceNo();
		m_type = pInvoice->GetInvoiceType();

		ICowInvoiceEntry* pEntry = pInvoice->GetCowAt(0);
		ICowTrail* pTrail = facade->GetCowTrail(pEntry->GetCowId());
		const ICowFootprint* pExitFootprint = pTrail->GetExit();
		IDoc* pExitDoc = pExitFootprint->GetDoc();		
		m_OutDocId = pExitDoc->GetId();

		m_hent.CopyFrom(pInvoice->GetInvoiceHent());
		m_details.CopyFrom(pInvoice);
				
		ICow* pCow;		
		uint32_t cowId;
		PtrEnumerator<ICowInvoiceEntry> entryEnum = pInvoice->EnumEntries();		
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			cowId = pEntry->GetCowId();
			pCow = facade->FindCow(cowId);
			pTrail = facade->GetCowTrail(cowId);

			pExitFootprint = pTrail->GetExit();
			pExitDoc = pExitFootprint->GetDoc();		

			InvoiceTransactionCowItem* item = new InvoiceTransactionCowItem(InvoiceItemKey(DocKey(pExitDoc->GetDocType(),pExitDoc->GetId()),ObtainNewItemId()),cowId);
			CowDetails cowDetails;
			cowDetails.CopyFrom(pCow);
			item->SetCow(cowDetails);

			CowInvoiceEntryDetails entryDetails;
			entryDetails.CopyFrom(pEntry);
			item->SetEntry(entryDetails);

			m_Items.push_back(item);
			entryEnum.advance();
		}
	});
}

uint32_t SellInvoiceCowsTransaction::GetId() const
{
	return GetTransactionId();
}

void SellInvoiceCowsTransaction::Commit()
{
	Do(	[this](SeasonSessionFacade* facade)
		{
			InvoiceSnapshot snapshot(m_invoiceId);
			snapshot.SetDetails(m_details);
			snapshot.SetInvoiceHent(m_hent);
			snapshot.SetInvoiceType(m_type);
						
			BOOST_FOREACH(InvoiceTransactionCowItem* item,m_Items)
			{
				InvoiceCowItem snaphotItem(item->GetCowId(),item->GetItemKey().GetDocKey().GetDocNo());
				snaphotItem.SetCow(item->GetCow());
				snaphotItem.SetEntry(item->GetEntry());
				snapshot.AddItem(snaphotItem);
			}	

			BOOST_FOREACH(DeductionTransactionItem* deductionItem, m_Deductions)
			{
				snapshot.AddDeduction(deductionItem->GetDeduction());
			}

			if(m_invoiceId == NULL_ID)
			{
				facade->InsertSellCowsInvoice(snapshot);
				facade->UnlockOutDoc(m_OutDocId);
			}
			else
			{
				facade->UpdateSellCowsInvoice(snapshot);
			}
		});	
	Cleanup();
}

void SellInvoiceCowsTransaction::Abort()
{
	if(m_invoiceId == NULL_ID)
	{
		Do([this](SeasonSessionFacade* facade)
		{
			facade->UnlockOutDoc(m_OutDocId);
		});
	}

	Cleanup();
}


