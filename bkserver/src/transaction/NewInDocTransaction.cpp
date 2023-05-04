#include "stdafx.h"
#include "NewInDocTransaction.h"
#include <boost/range/adaptor/map.hpp>
#include <boost/foreach.hpp>
#include "../ClientSeasonSession.h"
#include "../SeasonSession.h"
#include <data/runtime/InDocOperations.h>
#include <data/runtime/DocTransactionItemTuple.h>
//-------------------------------------------------------------------------------------------------

NewInDocTransaction::NewInDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId, uint32_t herdId) :
	PlusDocTransactionSkeleton(pInterface, logger,transactionId), m_HerdId(herdId)
{
}

boost::any NewInDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
{
	switch(operation)
	{
		case InDocOperations::Operation_SetDocDetails:
			SetDocDetails(args.get<DocDetails>(0));
			break;
		case InDocOperations::Operation_AddCow:
			return AddCow(args.get<CowDetails>(0));
		case InDocOperations::Operation_DeleteCow:
			DeleteCow(args.get<ItemId>(0));
			break;
		case InDocOperations::Operation_UpdateCow:
			UpdateCow(args.get<ItemId>(0),args.get<CowDetails>(1));
			break;
		case InDocOperations::Operation_UpdateEntry:
			UpdateEntry(args.get<ItemId>(0),args.get<CowEntryDetails>(1));
			break;
		case InDocOperations::Operation_GetItemsCount:
			return GetItemsCount();
		case InDocOperations::Operation_GetItemAt:
			{
				int index = args.get<int>(0);
				DocTransactionCowItem* pTransactionItem = GetItemAt(index);

				DocTransactionItemTuple item(pTransactionItem->GetId(),
										  pTransactionItem->GetCowId(),
										  pTransactionItem->GetCow(),				
										  pTransactionItem->GetEntry());
				
				return item;
			}
		case InDocOperations::Operation_GetDocId:
			return NULL_ID;	
		case InDocOperations::Operation_GetDocDetails:
			return m_docDetails;
		case InDocOperations::Operation_GetHerdId:
			return m_HerdId;
	}
	return boost::any();
}


uint32_t NewInDocTransaction::GetId() const
{
	return GetTransactionId();
}

void NewInDocTransaction::Commit()
{
	Do([this](SeasonSessionFacade* facade)
		{
			OwnDocSnapshot snapshot(NULL_ID);
			snapshot.SetDetails(m_docDetails);
			snapshot.SetHerdId(m_HerdId);

			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				snapshot.AddItem(*item);
			}	
			facade->InsertInDoc(snapshot);
		});	
}

void NewInDocTransaction::Abort()
{
	Clear();
}

