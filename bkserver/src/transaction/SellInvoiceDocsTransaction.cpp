#include "stdafx.h"
#include "SellInvoiceDocsTransaction.h"
#include <data/runtime/InvoiceDocsScopeOperations.h>
#include <data/runtime/InvoiceTransactionItemTuple.h>
#include <string/TextUtils.h>
#include "TransactionException.h"
#include <errortoken/ErrorToken.h>
#include <boost/foreach.hpp>
#include <arrays/DeepDelete.h>
#include "../snapshot/InvoiceSnapshot.h"
#include <data/impl/CowInvoiceEntryDetails.h>
#include <data/runtime/DeductionItemTuple.h>

SellInvoiceDocsTransaction::SellInvoiceDocsTransaction(ISeasonInterface* pInterface,uint32_t transactionId) : InvoiceTransactionSkeleton(pInterface,transactionId)		
{
}

boost::any SellInvoiceDocsTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
{
	switch(operation)
	{
		case InvoiceDocScopeOperations::Operation_SetHent:
			SetHent(args.get<InvoiceHent>(0));
			break;
		case InvoiceDocScopeOperations::Operation_GetHent:
			return m_hent;
		case InvoiceDocScopeOperations::Operation_GetInvoiceId:	
			return m_invoiceId;

		case InvoiceDocScopeOperations::Operation_GetInvoiceNo:
			return m_invoiceNo;

		case InvoiceDocScopeOperations::Operation_GetInvoiceDetails:
			return m_details;
		case InvoiceDocScopeOperations::Operation_SetInvoiceDetails:	
			m_details.CopyFrom(args.get<InvoiceDetails>(0));		
			break;
		case InvoiceDocScopeOperations::Operation_GetDocCount:
			return (count_t)m_DocIds.size();
		case InvoiceDocScopeOperations::Operation_GetDocAt:
			{
				int index = args.get<int>(0);
				auto it = m_DocIds.begin();
				std::advance(it,index);
				return *it;
			}
		case InvoiceDocScopeOperations::Operation_AddDoc:
			{
				uint32_t docId = args.get<uint32_t>(0);
				AddDoc(docId);
				break;
			}

		case InvoiceDocScopeOperations::Operation_DeleteDoc:		
			{
				uint32_t docId = args.get<uint32_t>(0);
				DeleteDoc(docId);
				break;
			}

		case InvoiceDocScopeOperations::Operation_GetDocItemAt:
			{
				uint32_t docId = args.get<uint32_t>(0);
				int index = args.get<int>(1);
				auto docItemsRange = GetDocItems(docId);
				auto itemIt = docItemsRange.begin();
				std::advance(itemIt,index);
				const InvoiceTransactionCowItem* item = *itemIt;
				InvoiceTransactionItemTuple itemTuple(item->GetItemKey(),item->GetCowId(),item->GetCow(),item->GetEntry());
				return itemTuple;
			}
		case InvoiceDocScopeOperations::Operation_GetDocItemCount:
			{
				uint32_t docId = args.get<uint32_t>(0);
				auto docItemsRange = GetDocItems(docId);
				size_t docItemCount = std::distance(docItemsRange.begin(),docItemsRange.end());
				return (count_t)docItemCount;
			}
		case InvoiceDocScopeOperations::Operation_UpdateCow:
			{
				ItemId id = args.get<ItemId>(0);
				CowDetails details = args.get<CowDetails>(1);
				break;
			}

		case InvoiceDocScopeOperations::Operation_UpdateCowInvoiceEntry:
			{
				ItemId id = args.get<ItemId>(0);
				CowInvoiceEntryDetails details = args.get<CowInvoiceEntryDetails>(1);
				InvoiceTransactionCowItem* item = AcquireItem(id);
				item->SetEntry(details);
				break;
			}
		case InvoiceDocScopeOperations::Operation_GetItemsCount:
			{
				return (count_t)m_Items.size();
			}
		case InvoiceDocScopeOperations::Operation_GetItemAt:		
			{
				int index = args.get<int>(0);
				InvoiceTransactionCowItem* item = m_Items[index];
				InvoiceTransactionItemTuple itemTuple(item->GetItemKey(),item->GetCowId(),item->GetCow(),item->GetEntry());
				return itemTuple;
			}	
		case InvoiceDocScopeOperations::Operation_GetDeductionsCount:
			{
					return (count_t)m_Deductions.size();
			}
		case InvoiceDocScopeOperations::Operation_GetDeductionAt:
			{
				int index = args.get<int>(0);
				DeductionTransactionItem* deduction = m_Deductions.item_at(index);
				return DeductionItemTuple(deduction->GetId(), deduction->GetDeduction());
			}

		case InvoiceDocScopeOperations::Operation_GetDeduction:
			{
				ItemId id = args.get<ItemId>(0);
				DeductionTransactionItem* deduction = m_Deductions.find_item(id);
				return DeductionItemTuple(deduction->GetId(), deduction->GetDeduction());
			}
		case InvoiceDocScopeOperations::Operation_AddDeduction:
			{
				return AddDeduction(args.get<InvoiceDeduction>(0));
			}
		case InvoiceDocScopeOperations::Operation_RemoveDeduction:
			{
					DeleteDeduction(args.get<ItemId>(0));
					break;
			}
		case InvoiceDocScopeOperations::Operation_GetInvoiceType:
			return m_type;

		case InvoiceDocScopeOperations::Operation_SetInvoiceType:
			m_type = args.get<InvoiceType>(0);
			break;
	}
	return boost::any();
}

SellInvoiceDocsTransaction::ItemsRange SellInvoiceDocsTransaction::GetDocItems(uint32_t docId)
{
	boost::multi_index::index<InvoiceTransactionItemsIndex, tag_docId>::type& indexByDoc = boost::multi_index::get<tag_docId>(m_Items);
	auto itemRange = indexByDoc.equal_range(docId);
	return SellInvoiceDocsTransaction::ItemsRange(itemRange.first,itemRange.second);
}

void SellInvoiceDocsTransaction::New()
{
	m_invoiceId	= NULL_ID;
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

void SellInvoiceDocsTransaction::Edit(uint32_t invoiceId)
{
	m_invoiceId = invoiceId;
	Do([this](SeasonSessionFacade* facade)
	{
		ISellInvoice* pInvoice = facade->FindSellInvoice(m_invoiceId);
		m_invoiceNo = pInvoice->GetInvoiceNo();
		m_type = pInvoice->GetInvoiceType();
		m_hent.CopyFrom(pInvoice->GetInvoiceHent());
		m_details.CopyFrom(pInvoice);

		
		ICow* pCow;
		ICowInvoiceEntry* pEntry;
		PtrEnumerator<ICowInvoiceEntry> entryEnum = pInvoice->EnumEntries();
		uint32_t cowId;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			cowId = pEntry->GetCowId();
			pCow = facade->FindCow(cowId);
			ICowTrail* pTrail = facade->GetCowTrail(cowId);
			const ICowFootprint* pExitFootprint = pTrail->GetExit();
			IDoc* pExitDoc = pExitFootprint->GetDoc();			
			m_DocIds.insert(pExitDoc->GetId());
			InvoiceTransactionCowItem* item = new InvoiceTransactionCowItem(InvoiceItemKey(DocKey(pExitDoc->GetDocType(),pExitDoc->GetId()),ObtainNewItemId()),cowId);
			CowDetails cowDetails;
			cowDetails.CopyFrom(pCow);
			item->SetCow(cowDetails);

			CowInvoiceEntryDetails exitDetails;
			exitDetails.CopyFrom(pEntry);
			item->SetEntry(exitDetails);

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

uint32_t SellInvoiceDocsTransaction::GetId() const
{
	return GetTransactionId();
}

void SellInvoiceDocsTransaction::Commit()
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
				facade->InsertSellDocsInvoice(snapshot);
				std::for_each(m_newlyAddedDocIds.begin(),m_newlyAddedDocIds.end(),std::bind(&SeasonSessionFacade::UnlockSellDoc,facade,std::placeholders::_1));				
			}
			else
			{
				facade->UpdateSellDocsInvoice(snapshot);
				std::for_each(m_deletedDocIds.begin(),m_deletedDocIds.end(),std::bind(&SeasonSessionFacade::UnlockSellDoc,facade,std::placeholders::_1));				
			}

		});	
	Cleanup();
}

void SellInvoiceDocsTransaction::UnlockNewlyAddedDocs()
{
	Do([this](SeasonSessionFacade* facade)
	{
		BOOST_FOREACH(uint32_t docId,m_newlyAddedDocIds)
		{
			facade->UnlockSellDoc(docId);
		}
	});
}

void SellInvoiceDocsTransaction::Abort()
{
	UnlockNewlyAddedDocs();		
	Cleanup();
}

void SellInvoiceDocsTransaction::AddDoc(uint32_t docId)
{
	Do([this,docId](SeasonSessionFacade* facade)
	{
		ISellDoc* pSellDoc = facade->FindSellDoc(docId);		

		if(m_DocIds.end() != m_DocIds.find(docId))
			BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"sell doc with id=%d is already a part of this invoice",docId)));

		if(pSellDoc->GetInvoiceId() != NULL_ID)
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"sell_doc_has_invoice")(L"doc_id",docId)(L"invoice_id", pSellDoc->GetInvoiceId()).str()));

		facade->LockSellDoc(docId);

		ICow* pCow;
		ICowEntry* pEntry;
		PtrEnumerator<ICowEntry> cowsEnum = pSellDoc->EnumCows();
		while(cowsEnum.hasNext())
		{
			pEntry = *cowsEnum;
			pCow = facade->FindCow(pEntry->GetCowId());

			InvoiceTransactionCowItem* item = new InvoiceTransactionCowItem(InvoiceItemKey(DocKey(pSellDoc->GetDocType(),pSellDoc->GetId()),ObtainNewItemId()),pEntry->GetCowId());
			CowDetails cowDetails;
			cowDetails.CopyFrom(pCow);
			item->SetCow(cowDetails);
			m_Items.push_back(item);
			cowsEnum.advance();
		}
		m_DocIds.insert(docId);
		m_newlyAddedDocIds.insert(docId);
	});
}

void SellInvoiceDocsTransaction::DeleteDoc(uint32_t docId)
{
	if(m_DocIds.end() == m_DocIds.find(docId))
			BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"sell doc with id=%d is not a part of this invoice",docId)));	

	if(m_newlyAddedDocIds.find(docId) != m_newlyAddedDocIds.end())
	{
		Do([this,docId](SeasonSessionFacade* facade)
		{
			facade->UnlockSellDoc(docId);			
		});
		m_newlyAddedDocIds.erase(docId);
	}
	else
	{
		m_deletedDocIds.insert(docId);
	}

	m_DocIds.erase(docId);
	DeleteDocItems(docId);
}

