#include "stdafx.h"
#include "NewBuyDocTransaction.h"
#include <boost/range/adaptor/map.hpp>
#include <boost/foreach.hpp>
#include "../ClientSeasonSession.h"
#include "../SeasonSession.h"
#include <data/runtime/BuyDocOperations.h>
#include "TransactionException.h"
#include <data/runtime/DocTransactionItemTuple.h>


NewBuyDocTransaction::NewBuyDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId, uint32_t herdId) : 
		PlusDocTransactionSkeleton(pInterface, logger,transactionId), m_HerdId(herdId), m_HentId(NULL_ID)
{
}

boost::any NewBuyDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
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
				return DocTransactionItemTuple( pTransactionItem->GetId(),
												pTransactionItem->GetCowId(),
												pTransactionItem->GetCow(),
												pTransactionItem->GetEntry());
			}

		case BuyDocOperations::Operation_GetItem:
			{
				ItemId id = args.get<ItemId>(0);
				DocTransactionCowItem* pTransactionItem = FindItem(id);

				return DocTransactionItemTuple( pTransactionItem->GetId(),
												pTransactionItem->GetCowId(),
												pTransactionItem->GetCow(),
												pTransactionItem->GetEntry());
			}
		case BuyDocOperations::Operation_GetDocId:
			return NULL_ID;	
		case BuyDocOperations::Operation_GetDocDetails:
			return m_docDetails;
		case BuyDocOperations::Operation_SetHent:
			{
				m_HentId = args.get<uint32_t>(0);
				break;
			}
		case BuyDocOperations::Operation_GetHent:
			return m_HentId;
		case BuyDocOperations::Operation_GetHerd:
			return m_HerdId;
		default:
			BOOST_THROW_EXCEPTION(TransactionException(L"unsupported operation"));
	}
	return boost::any();
}

uint32_t NewBuyDocTransaction::GetId() const
{
	return GetTransactionId();
}

void NewBuyDocTransaction::Commit()
{
	Do([this](SeasonSessionFacade* facade)
		{
			HentsDocSnapshot snapshot(NULL_ID);
			snapshot.SetDetails(m_docDetails);
			snapshot.SetHerdId(m_HerdId);
			snapshot.SetHentId(m_HentId);

			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				snapshot.AddItem(*item);
			}	
			facade->InsertBuyDoc(snapshot);
		});	
}

void NewBuyDocTransaction::Abort()
{
	Clear();
}

