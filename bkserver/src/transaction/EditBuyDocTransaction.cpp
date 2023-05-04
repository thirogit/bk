#include "stdafx.h"
#include "EditBuyDocTransaction.h"
#include <data/runtime/BuyDocOperations.h>
#include <boost/range/adaptors.hpp>
#include <boost/foreach.hpp>
#include <boost/range/algorithm.hpp>
#include "TransactionException.h"

#include <data/runtime/DocTransactionItemTuple.h>
#include <errortoken/ErrorToken.h>

//-------------------------------------------------------------------------------------------------

EditBuyDocTransaction::EditBuyDocTransaction(ISeasonInterface* pInterface, LogInterface* logger,uint32_t transactionId) : PlusDocTransactionSkeleton(pInterface, logger, transactionId), m_herdId(NULL_ID), m_docId(NULL_ID), m_hentId(NULL_ID)
{
	m_bInvoiced = false;
}

void EditBuyDocTransaction::Edit(uint32_t docId)
{
	m_docId = docId;
	Do([this](SeasonSessionFacade* facade)
			  {
					IBuyDoc* pDoc = facade->FindBuyDoc(m_docId);					
					PtrEnumerator<ICowEntry> cowEnum = pDoc->EnumCows();
					while(cowEnum.hasNext())
					{
						ICowEntry* pEntry = *cowEnum;
						ICow* pCow = facade->FindCow(pEntry->GetCowId());
						CowDetails details;
						details.CopyFrom(pCow);
						this->DocTransactionSkeleton::AddCow(pCow->GetId(),details);
						cowEnum.advance();
					}
					m_herdId = pDoc->GetHerdId();
					m_docDetails.CopyFrom(pDoc);
					m_hentId = pDoc->GetHentId();
					m_bInvoiced = pDoc->GetInvoiceId() != NULL_ID;
			  });
}


boost::any EditBuyDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
{
	switch(operation)
	{
		case BuyDocOperations::Operation_SetDocDetails:
			SetDocDetails(args.get<DocDetails>(0));
			break;
		case BuyDocOperations::Operation_AddCow:
			return AddCow(args.get<CowDetails>(0));
		case BuyDocOperations::Operation_DeleteCow:
			DeleteCow(args.get<ItemId>(0));
			break;
		case BuyDocOperations::Operation_UpdateCow:
			UpdateCow(args.get<ItemId>(0),args.get<CowDetails>(1));
			break;
		case BuyDocOperations::Operation_UpdateEntry:
			UpdateEntry(args.get<ItemId>(0),args.get<CowEntryDetails>(1));
			break;
		case BuyDocOperations::Operation_GetItemsCount:
			return GetItemsCount();
		case BuyDocOperations::Operation_GetItemAt:
			{
				int index = args.get<int>(0);
				DocTransactionCowItem* pTransactionItem = GetItemAt(index);

				DocTransactionItemTuple item(pTransactionItem->GetId(),
										  pTransactionItem->GetCowId(),
										  pTransactionItem->GetCow(),				
										  pTransactionItem->GetEntry());
				
				return item;
			}	
		case BuyDocOperations::Operation_GetDocId:
			return m_docId;	
		case BuyDocOperations::Operation_GetDocDetails:
			return m_docDetails;
		case BuyDocOperations::Operation_GetHerd:
			return m_herdId;
		case BuyDocOperations::Operation_GetHent:
			return m_hentId;
		case BuyDocOperations::Operation_SetHent:
			m_hentId = args.get<uint32_t>(0);
			break;
	}
	return boost::any();
}

uint32_t EditBuyDocTransaction::GetId() const
{
	return GetTransactionId();
}

void EditBuyDocTransaction::DeleteCow(ItemId id)
{
	if(m_bInvoiced)
		BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cannot_delete_buydoc_has_invoice").str()));

	PlusDocTransactionSkeleton::DeleteCow(id);
}

ItemId EditBuyDocTransaction::AddCow(CowDetails details)
{
	if(m_bInvoiced)
		BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cannot_add_buydoc_has_invoice").str()));

	return PlusDocTransactionSkeleton::AddCow(details);
}

void EditBuyDocTransaction::Commit()
{
	Do([this](SeasonSessionFacade* facade)
		{
			HentsDocSnapshot snapshot(m_docId);
			snapshot.SetDetails(m_docDetails);
			snapshot.SetHerdId(m_herdId);
			snapshot.SetHentId(m_hentId);
			
			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				snapshot.AddItem(*item);
			}	
			facade->UpdateBuyDoc(snapshot);
		});	
}

void EditBuyDocTransaction::Abort()
{
	Clear();
}



