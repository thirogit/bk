#include "stdafx.h"
#include "BuyInvoiceDocsTransaction.h"
#include <data/runtime/InvoiceDocsScopeOperations.h>
#include <data/runtime/InvoiceTransactionItemTuple.h>
#include <string/TextUtils.h>
#include "TransactionException.h"
#include <errortoken/ErrorToken.h>
#include <boost/foreach.hpp>
#include <arrays/DeepDelete.h>
#include "../snapshot/InvoiceSnapshot.h"
#include <data/impl/CowInvoiceEntryDetails.h>
#include <CommonTypes.h>
#include <data/runtime/DeductionItemTuple.h>

BuyInvoiceDocsTransaction::BuyInvoiceDocsTransaction(ISeasonInterface* pInterface,uint32_t transactionId) : InvoiceTransactionSkeleton(pInterface,transactionId)		
{
}

boost::any BuyInvoiceDocsTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
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
			{
				count_t docCount = m_DocIds.size();
				return docCount;
			}			
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
				count_t docItemCount = std::distance(docItemsRange.begin(),docItemsRange.end());
				return docItemCount;
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
				count_t itemCount = m_Items.size();
				return itemCount;
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

BuyInvoiceDocsTransaction::ItemsRange BuyInvoiceDocsTransaction::GetDocItems(uint32_t docId)
{
	boost::multi_index::index<InvoiceTransactionItemsIndex, tag_docId>::type& indexByDoc = boost::multi_index::get<tag_docId>(m_Items);
	auto itemRange = indexByDoc.equal_range(docId);
	return BuyInvoiceDocsTransaction::ItemsRange(itemRange.first,itemRange.second);
}

void BuyInvoiceDocsTransaction::New()
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

void BuyInvoiceDocsTransaction::Edit(uint32_t invoiceId)
{
	m_invoiceId = invoiceId;
	Do([this](SeasonSessionFacade* facade)
	{
		IBuyInvoice* pInvoice = facade->FindBuyInvoice(m_invoiceId);
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
			const ICowFootprint* pEntryFootprint = pTrail->GetEntry();
			IDoc* pEntryDoc = pEntryFootprint->GetDoc();			
			m_DocIds.insert(pEntryDoc->GetId());
			InvoiceTransactionCowItem* item = new InvoiceTransactionCowItem(InvoiceItemKey(DocKey(pEntryDoc->GetDocType(),pEntryDoc->GetId()),ObtainNewItemId()),cowId);
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

uint32_t BuyInvoiceDocsTransaction::GetId() const
{
	return GetTransactionId();
}

void BuyInvoiceDocsTransaction::Commit()
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
				facade->InsertBuyDocsInvoice(snapshot);
				std::for_each(m_newlyAddedDocIds.begin(),m_newlyAddedDocIds.end(),std::bind(&SeasonSessionFacade::UnlockBuyDoc,facade,std::placeholders::_1));				
			}
			else
			{
				facade->UpdateBuyDocsInvoice(snapshot);
				std::for_each(m_deletedDocIds.begin(),m_deletedDocIds.end(),std::bind(&SeasonSessionFacade::UnlockBuyDoc,facade,std::placeholders::_1));				
			}

		});	
	Cleanup();
}

void BuyInvoiceDocsTransaction::UnlockNewlyAddedDocs()
{
	Do([this](SeasonSessionFacade* facade)
	{
		BOOST_FOREACH(uint32_t docId,m_newlyAddedDocIds)
		{
			facade->UnlockBuyDoc(docId);
		}
	});
}

void BuyInvoiceDocsTransaction::Abort()
{
	UnlockNewlyAddedDocs();		
	Cleanup();
}

void BuyInvoiceDocsTransaction::AddDoc(uint32_t docId)
{
	Do([this,docId](SeasonSessionFacade* facade)
	{
		IBuyDoc* pBuyDoc = facade->FindBuyDoc(docId);		

		if(m_DocIds.end() != m_DocIds.find(docId))
			BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"buy doc with id=%d is already a part of this invoice",docId)));

		if(pBuyDoc->GetInvoiceId() != NULL_ID)
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"buy_doc_has_invoice")(L"doc_id",docId).str()));

		facade->LockBuyDoc(docId);

		ICow* pCow;
		ICowEntry* pEntry;
		PtrEnumerator<ICowEntry> cowsEnum = pBuyDoc->EnumCows();
		while(cowsEnum.hasNext())
		{
			pEntry = *cowsEnum;
			pCow = facade->FindCow(pEntry->GetCowId());

			InvoiceTransactionCowItem* item = new InvoiceTransactionCowItem(InvoiceItemKey(DocKey(pBuyDoc->GetDocType(),pBuyDoc->GetId()),ObtainNewItemId()),pEntry->GetCowId());
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

void BuyInvoiceDocsTransaction::DeleteDoc(uint32_t docId)
{
	if(m_DocIds.end() == m_DocIds.find(docId))
			BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"buy doc with id=%d is not a part of this invoice",docId)));	

	if(m_newlyAddedDocIds.find(docId) != m_newlyAddedDocIds.end())
	{
		Do([this,docId](SeasonSessionFacade* facade)
		{
			facade->UnlockBuyDoc(docId);			
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

