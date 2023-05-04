#include "stdafx.h"
#include "EditMoveDocTransaction.h"
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

EditMoveDocTransaction::EditMoveDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId) : DocTransactionSkeleton(pInterface, logger,transactionId)
{
}

uint32_t EditMoveDocTransaction::GetId() const
{
	return GetTransactionId();
}

boost::any EditMoveDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
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
		
		case MoveDocOperations::Operation_GetDocId:
			return m_docId;	
		case MoveDocOperations::Operation_GetDocDetails:
			return m_docDetails;
		case MoveDocOperations::Operation_AddCow:
			return AddCow(args.get<uint32_t>(0));	
			break;
		case MoveDocOperations::Operation_GetSrcHerdId:
			return m_srcHerdId;
		case MoveDocOperations::Operation_GetDstHerdId:
			return m_dstHerdId;
		
	}
	return boost::any();
}

void EditMoveDocTransaction::Edit(uint32_t docId)
{
	m_docId = docId;
	Do([this](SeasonSessionFacade* facade)
			  {
					IMoveDoc* pDoc = facade->FindMoveDoc(m_docId);					
					PtrEnumerator<ICowEntry> cowEnum = pDoc->EnumCows();
					while(cowEnum.hasNext())
					{
						ICowEntry* pEntry = *cowEnum;
						ICow* pCow = facade->FindCow(pEntry->GetCowId());
						CowDetails details;
						details.CopyFrom(pCow);
						ItemId id = DocTransactionSkeleton::AddCow(pCow->GetId(),details);

						DocTransactionCowItem* pItem = FindItem(id);

						CowEntryDetails entryDetails;
						entryDetails.CopyFrom(pEntry);
						pItem->SetEntry(entryDetails);
						cowEnum.advance();
					}
					m_srcHerdId = pDoc->GetSrcHerdId();
					m_dstHerdId = pDoc->GetDstHerdId();
					m_docDetails.CopyFrom(pDoc);
			  });
}

ItemId EditMoveDocTransaction::AddCow(uint32_t cowId)
{
	if(FindItem([cowId](DocTransactionCowItem* item) -> bool
				{
					return item->GetCowId() == cowId;
				}))
	{
		BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"cow with id = %d already belongs to this transaction",cowId)));
	}
					
	return Eval<ItemId>([this,cowId](SeasonSessionFacade* facade) -> ItemId
						{
							ICowTrail* pTrail = facade->GetCowTrail(cowId);
							CowPath path = pTrail->GetPath();

							IHerdFootprint* pLastStepInSrcHerd = path.GetLastStepInHerd(m_srcHerdId);
							
							if(!pLastStepInSrcHerd)
								BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"cow(id = %d) was never present in src herd(id=%d)",cowId,m_srcHerdId)));

							if(pLastStepInSrcHerd->NextStep())
							{
								BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"cow(id = %d) is not in stock in src herd(id=%d)",cowId,m_srcHerdId)));
							}
							
							facade->LockCow(cowId);
							const ICow* pCow = facade->FindCow(cowId);								
																		
							CowDetails details;
							details.CopyFrom(pCow);


							ItemId newlyAddedItemId = DocTransactionSkeleton::AddCow(cowId,details);
							m_newlyAddedItems.add_item(FindItem(newlyAddedItemId));
							
							return newlyAddedItemId;
						});	
}

void EditMoveDocTransaction::Commit()
{
	Do([this](SeasonSessionFacade* facade)
		{
			MoveDocSnapshot snapshot(m_docId);
			snapshot.SetDetails(m_docDetails);
			snapshot.SetDstHerdId(m_dstHerdId);
			snapshot.SetSrcHerdId(m_srcHerdId);
			
			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				snapshot.AddItem(*item);
			}	

			facade->UpdateMoveDoc(snapshot);																				
												
		});	
	Clear();
}

void EditMoveDocTransaction::Abort()
{
	Do(	[this](SeasonSessionFacade* facade)
		{										
			BOOST_FOREACH(DocTransactionCowItem* item,m_newlyAddedItems)
			{
				facade->UnlockCow(item->GetCowId());
			}	
											
		});

	Clear();
}

void EditMoveDocTransaction::DeleteCow(ItemId id)
{
	DocTransactionCowItem* pItem = FindItem(id);		

	Do([this,pItem,id](SeasonSessionFacade* facade) 
		{	
			if(m_newlyAddedItems.contains_item(id))
			{
				m_newlyAddedItems.remove_item(id);	
												
				facade->UnlockCow(pItem->GetCowId());															
			}
			else
			{
				ICowPresence* pPresence = facade->GetCowPresence(m_dstHerdId,CowPresenceKey(pItem->GetCowId(),DocType_Move,m_docId));

				if(!pPresence)
					BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"cow(id = %d) was never present in dst herd(id=%d) via %c%05d",pItem->GetCowId(),m_dstHerdId,DocType_Move,m_docId)));

				if(!pPresence->IsInStock())
				{
					const IDoc* pHerdMinusDoc = pPresence->GetHerdOutDoc();
					BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"cow(id = %d) was moved further from dst herd(id=%d) via %c%05d",pItem->GetCowId(),m_dstHerdId,pHerdMinusDoc->GetDocType(),pHerdMinusDoc->GetDocType())));
				}
			}
			DocTransactionSkeleton::DeleteCow(pItem->GetId());
		});
}

