#include "stdafx.h"
#include "OutDocTransaction.h"
#include <data/runtime/OutDocOperations.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include "../ClientSeasonSession.h"
#include "../SeasonSession.h"
#include "../snapshot/MoveDocSnapshot.h"
#include "TransactionException.h"
#include <string/TextUtils.h>
#include <data/runtime/DocTransactionItemTuple.h>
//-------------------------------------------------------------------------------------------------

OutDocTransaction::OutDocTransaction(ISeasonInterface* pInterface,uint32_t transactionId) : DocTransactionSkeleton(pInterface,transactionId)
{
}

uint32_t OutDocTransaction::GetId() const
{
	return GetTransactionId();
}

boost::any OutDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
{
	switch(operation)
	{
		case OutDocOperations::Operation_SetDocDetails:
			SetDocDetails(args.get<DocDetails>(0));
			break;		
		case OutDocOperations::Operation_DeleteCow:
			DeleteCow(args.get<ItemId>(0));
			break;		
		case OutDocOperations::Operation_UpdateEntry:
			UpdateEntry(args.get<ItemId>(0),args.get<CowEntryDetails>(1));
			break;
		case OutDocOperations::Operation_GetItemsCount:
			return GetItemsCount();
		case OutDocOperations::Operation_GetItemAt:			
			{
				int index = args.get<int>(0);
				DocTransactionCowItem* pTransactionItem = GetItemAt(index);

				DocTransactionItemTuple item(pTransactionItem->GetItemId(),
										  pTransactionItem->GetCowId(),
										  pTransactionItem->GetCow(),				
										  pTransactionItem->GetEntry());
				
				return item;
			}

		case OutDocOperations::Operation_GetItem:
			{
				ItemId id = args.get<ItemId>(0);
				DocTransactionCowItem* pTransactionItem = FindItem(id);

				DocTransactionItemTuple item(pTransactionItem->GetItemId(),
										  pTransactionItem->GetCowId(),
										  pTransactionItem->GetCow(),				
										  pTransactionItem->GetEntry());
				
				return item;
			}
		
		case OutDocOperations::Operation_GetDocId:
			return m_docId;	
		case OutDocOperations::Operation_GetDocDetails:
			return m_docDetails;
		case OutDocOperations::Operation_AddCow:
			return AddCow(args.get<uint32_t>(0));	
			break;
		case OutDocOperations::Operation_GetHerd:
			return m_herdId;		
		
		
	}
	return boost::any();
}

void OutDocTransaction::New(uint32_t herdId)
{
	m_docId = NULL_ID;
	m_herdId = herdId;
}

void OutDocTransaction::Edit(uint32_t docId)
{
	m_docId = docId;
	Do([this](SeasonSessionFacade* facade)
			  {
					IOutDoc* pDoc = facade->FindOutDoc(m_docId);					
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
					m_herdId = pDoc->GetHerdId();
					
					m_docDetails.CopyFrom(pDoc);
			  });
}

ItemId OutDocTransaction::AddCow(uint32_t cowId)
{
	if(FindItem([cowId](DocTransactionCowItem* item) -> bool
				{
					return item->GetCowId() == cowId;
				}))
	{
		throw TransactionException(TextUtils::FormatA("cow with id = %d already belongs to this transaction",cowId));
	}
					
	return Eval<ItemId>([this,cowId](SeasonSessionFacade* facade) -> ItemId
						{
							ICowTrail* pTrail = facade->GetCowTrail(cowId);
							CowPath path = pTrail->GetPath();

							IHerdFootprint* pLastStepInSrcHerd = path.GetLastStepInHerd(m_herdId);
							
							if(!pLastStepInSrcHerd)
								throw TransactionException(TextUtils::FormatA("cow(id = %d) was never present in herd(id=%d)",cowId,m_herdId));

							if(pLastStepInSrcHerd->NextStep())
							{
								throw TransactionException(TextUtils::FormatA("cow(id = %d) is not in stock in herd(id=%d)",cowId,m_herdId));
							}
							
							facade->LockCow(cowId);
							const ICow* pCow = facade->FindCow(cowId);								
																		
							CowDetails details;
							details.CopyFrom(pCow);

							ItemId newlyAddedItemId = DocTransactionSkeleton::AddCow(cowId,details);
							
							m_newlyAddedItems.add(FindItem(newlyAddedItemId));

							return newlyAddedItemId;
						});	
}

void OutDocTransaction::Commit()
{
	Do(	[this](SeasonSessionFacade* facade)
		{
			OwnDocSnapshot snapshot(m_docId);
			snapshot.SetDetails(m_docDetails);				
			snapshot.SetHerdId(m_herdId);
			
			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				snapshot.AddItem(*item);
			}	

			if(m_docId == NULL_ID)
			{
				facade->InsertOutDoc(snapshot);	

				BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
				{
					facade->UnlockCow(item->GetCowId());
				}
			}
			else
			{
				facade->UpdateOutDoc(snapshot);
			}												
		});	
	Clear();
}

void OutDocTransaction::Abort()
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

void OutDocTransaction::DeleteCow(ItemId id)
{
	DocTransactionCowItem* pItem = FindItem(id);

	if(m_newlyAddedItems.contains_item(id))
	{
		m_newlyAddedItems.remove(id);	

		Do(	[this,pItem](SeasonSessionFacade* facade)
		{										
			facade->UnlockCow(pItem->GetCowId());											
		});
	}
		
	DocTransactionSkeleton::DeleteCow(pItem->GetItemId());			
}

