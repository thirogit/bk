#include "stdafx.h"
#include "NewMoveDocTransaction.h"
#include <data/runtime/MoveDocOperations.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include "../ClientSeasonSession.h"
#include "../SeasonSession.h"
#include "../snapshot/MoveDocSnapshot.h"
#include "TransactionException.h"
#include <string/TextUtils.h>
#include <data/runtime/DocTransactionItemTuple.h>
//-------------------------------------------------------------------------------------------------

NewMoveDocTransaction::NewMoveDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId, uint32_t srcHerdId, uint32_t dstHerdId) : DocTransactionSkeleton(pInterface,logger, transactionId), m_srcHerdId(srcHerdId), m_dstHerdId(dstHerdId)
{
}

uint32_t NewMoveDocTransaction::GetId() const
{
	return GetTransactionId();
}

boost::any NewMoveDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
{
	switch(operation)
	{
		case MoveDocOperations::Operation_SetDocDetails:
			SetDocDetails(args.get<DocDetails>(0));
			break;		
		case MoveDocOperations::Operation_DeleteCow:
			DeleteCow(args.get<ItemId>(0));
			break;
		case MoveDocOperations::Operation_UpdateEntry:
			UpdateEntry(args.get<ItemId>(0),args.get<CowEntryDetails>(1));
			break;
		case MoveDocOperations::Operation_GetItemsCount:
			return GetItemsCount();
		case MoveDocOperations::Operation_GetItemAt:
			{
				int index = args.get<int>(0);
				DocTransactionCowItem* pTransactionItem = GetItemAt(index);

				DocTransactionItemTuple item(pTransactionItem->GetId(),
										  pTransactionItem->GetCowId(),
										  pTransactionItem->GetCow(),				
										  pTransactionItem->GetEntry());	

				return item;
			}
		
		case MoveDocOperations::Operation_GetDocId:
			return NULL_ID;	
		case MoveDocOperations::Operation_GetDocDetails:
			return m_docDetails;
		case MoveDocOperations::Operation_AddCow:
			return AddCow(	args.get<uint32_t>(0));	
			break;
		case MoveDocOperations::Operation_GetSrcHerdId:
			return m_srcHerdId;
		case MoveDocOperations::Operation_GetDstHerdId:
			return m_dstHerdId;
		case MoveDocOperations::Operation_GetItem:
			{
				ItemId id = args.get<ItemId>(0);
				DocTransactionCowItem* pTransactionItem = FindItem(id);

				DocTransactionItemTuple item(pTransactionItem->GetId(),
										  pTransactionItem->GetCowId(),
										  pTransactionItem->GetCow(),				
										  pTransactionItem->GetEntry());	

				return item;
			}
		
	}
	return boost::any();
}


ItemId NewMoveDocTransaction::AddCow(uint32_t cowId)
{
	//check if not already added
	//check if present in src herd

	return Eval<ItemId>([this,cowId](SeasonSessionFacade* facade) -> ItemId
						{
							facade->LockCow(cowId);
							const ICow* pCow = facade->FindCow(cowId);
							CowDetails details;
							details.CopyFrom(pCow);
							return DocTransactionSkeleton::AddCow(cowId,details);
						});	
}

void NewMoveDocTransaction::Commit()
{
	Do( [this](SeasonSessionFacade* facade)
		{
			MoveDocSnapshot snapshot(NULL_ID);
			snapshot.SetDetails(m_docDetails);
			snapshot.SetSrcHerdId(m_srcHerdId);
			snapshot.SetDstHerdId(m_dstHerdId);
			NewPtrFlushArray<CowServerObject> cowSOs;

			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				snapshot.AddItem(*item);
			}	

			facade->InsertMoveDoc(snapshot);

			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				facade->UnlockCow(item->GetCowId());
			}											
												
		});	
}

void NewMoveDocTransaction::Abort()
{

	Do(	[this](SeasonSessionFacade* facade)
		{										
			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				facade->UnlockCow(item->GetCowId());
			}	
											
		});

	Clear();
}


