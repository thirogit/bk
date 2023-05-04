#include "stdafx.h"
#include "OutDocTransaction.h"
#include <data/runtime/OutDocOperations.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/throw_exception.hpp>
#include "../ClientSeasonSession.h"
#include "../SeasonSession.h"
#include "../snapshot/MoveDocSnapshot.h"
#include "TransactionException.h"
#include <string/TextUtils.h>
#include <data/runtime/DocTransactionItemTuple.h>
#include <errortoken/ErrorToken.h>
//-------------------------------------------------------------------------------------------------

OutDocTransaction::OutDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId) : DocTransactionSkeleton(pInterface, logger,transactionId)
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
		{
			ItemId cowItemId = args.get<ItemId>(0);			
			DeleteCow(cowItemId);
			break;
		}
		case OutDocOperations::Operation_UpdateEntry:
			UpdateEntry(args.get<ItemId>(0),args.get<CowEntryDetails>(1));
			break;
		case OutDocOperations::Operation_GetItemsCount:
			return GetItemsCount();
		case OutDocOperations::Operation_GetItemAt:			
			{
				int index = args.get<int>(0);
				DocTransactionCowItem* pTransactionItem = GetItemAt(index);

				DocTransactionItemTuple item(pTransactionItem->GetId(),
										  pTransactionItem->GetCowId(),
										  pTransactionItem->GetCow(),				
										  pTransactionItem->GetEntry());
				
				return item;
			}

		case OutDocOperations::Operation_GetItem:
			{
				ItemId id = args.get<ItemId>(0);
				DocTransactionCowItem* pTransactionItem = FindItem(id);

				DocTransactionItemTuple item(pTransactionItem->GetId(),
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
		BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"cow with id = %d already belongs to this transaction",cowId)));
	}
					
	return Eval<ItemId>([this,cowId](SeasonSessionFacade* facade) -> ItemId
						{
							ICowTrail* pTrail = facade->GetCowTrail(cowId);
							CowPath path = pTrail->GetPath();

							IHerdFootprint* pLastStepInSrcHerd = path.GetLastStepInHerd(m_herdId);
							
							if(!pLastStepInSrcHerd)
								BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"cow(id = %d) was never present in herd(id=%d)",cowId,m_herdId)));

							if(pLastStepInSrcHerd->NextStep())
							{
								BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FormatA(L"cow(id = %d) is not in stock in herd(id=%d)",cowId,m_herdId)));
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
		m_newlyAddedItems.remove_item(id);	

		Do(	[this,pItem](SeasonSessionFacade* facade)
		{										
			facade->UnlockCow(pItem->GetCowId());											
		});
	}
	else
	{
		ThrowIfCowHasSellInvoice(pItem);
	}
		
	DocTransactionSkeleton::DeleteCow(pItem->GetId());			
}

void OutDocTransaction::ThrowIfCowHasSellInvoice(DocTransactionCowItem* pItem)
{
	Do([this, pItem](SeasonSessionFacade* facade)
	{
		ICow* cow = facade->FindCow(pItem->GetCowId());
		if (cow->GetSellInvoiceId() != NULL_ID)
		{
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"cow_has_sell_invoice")(L"cow_id", pItem->GetCowId())(L"invoice_id", cow->GetSellInvoiceId()).str()));
		}
	});
}