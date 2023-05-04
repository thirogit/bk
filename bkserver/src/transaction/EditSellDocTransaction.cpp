#include "stdafx.h"
#include "EditSellDocTransaction.h"
#include <data/runtime/SellDocOperations.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include "../ClientSeasonSession.h"
#include "../SeasonSession.h"
#include "../snapshot/MoveDocSnapshot.h"
#include "TransactionException.h"
#include <string/TextUtils.h>
#include <data/runtime/DocTransactionItemTuple.h>
#include <errortoken/ErrorToken.h>
//-------------------------------------------------------------------------------------------------

EditSellDocTransaction::EditSellDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId) : 
			DocTransactionSkeleton(pInterface, logger,transactionId),
			m_hentId(NULL_ID),
			m_docId(NULL_ID),
			m_herdId(NULL_ID),
			m_sellInvoiceId(NULL_ID)

{
}

uint32_t EditSellDocTransaction::GetId() const
{
	return GetTransactionId();
}

boost::any EditSellDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
{
	switch(operation)
	{
		case SellDocOperations::Operation_SetDocDetails:
			SetDocDetails(args.get<DocDetails>(0));
			break;		
		case SellDocOperations::Operation_DeleteCow:
			ThrowIfThereIsInvoice();
			DeleteCow(args.get<ItemId>(0));
			break;		
		case SellDocOperations::Operation_UpdateEntry:
			UpdateEntry(args.get<ItemId>(0),args.get<CowEntryDetails>(1));
			break;
		case SellDocOperations::Operation_GetItemsCount:
			return GetItemsCount();
		case SellDocOperations::Operation_GetItemAt:			
			{
				int index = args.get<int>(0);
				DocTransactionCowItem* pTransactionItem = GetItemAt(index);

				DocTransactionItemTuple item(pTransactionItem->GetId(),
										  pTransactionItem->GetCowId(),
										  pTransactionItem->GetCow(),				
										  pTransactionItem->GetEntry());
				
				return item;
			}

		case SellDocOperations::Operation_GetItem:
			{
				ItemId id = args.get<ItemId>(0);
				DocTransactionCowItem* pTransactionItem = FindItem(id);

				DocTransactionItemTuple item(pTransactionItem->GetId(),
										  pTransactionItem->GetCowId(),
										  pTransactionItem->GetCow(),				
										  pTransactionItem->GetEntry());
				
				return item;
			}
		
		case SellDocOperations::Operation_GetDocId:
			return m_docId;	
		case SellDocOperations::Operation_GetDocDetails:
			return m_docDetails;
		case SellDocOperations::Operation_AddCow:
			ThrowIfThereIsInvoice();
			return AddCow(args.get<uint32_t>(0));	
			break;
		case SellDocOperations::Operation_GetHerd:
			return m_herdId;		
		case SellDocOperations::Operation_SetHent:
			m_hentId = args.get<uint32_t>(0);
			break;
		case SellDocOperations::Operation_GetHent:
			return m_hentId;		
		
	}
	return boost::any();
}



void EditSellDocTransaction::Edit(uint32_t docId)
{
	m_docId = docId;
	Do([this](SeasonSessionFacade* facade)
			  {
					ISellDoc* pDoc = facade->FindSellDoc(m_docId);					
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
					m_hentId = pDoc->GetHentId();
					m_sellInvoiceId = pDoc->GetInvoiceId();
					m_docDetails.CopyFrom(pDoc);
			  });
}

ItemId EditSellDocTransaction::AddCow(uint32_t cowId)
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

void EditSellDocTransaction::Commit()
{
	Do([this](SeasonSessionFacade* facade)
		{
			HentsDocSnapshot snapshot(m_docId);
			snapshot.SetDetails(m_docDetails);	
			snapshot.SetHentId(m_hentId);
			snapshot.SetHerdId(m_herdId);
			
			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				snapshot.AddItem(*item);
			}	

			facade->UpdateSellDoc(snapshot);																				
												
		});	
	Clear();
}

void EditSellDocTransaction::Abort()
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

void EditSellDocTransaction::DeleteCow(ItemId id)
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
	
		
	DocTransactionSkeleton::DeleteCow(pItem->GetId());			
}

void EditSellDocTransaction::ThrowIfThereIsInvoice()
{
	if (m_sellInvoiceId != NULL_ID)
	{
		BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"sell_doc_has_invoice")(L"doc_id", m_docId)(L"invoice_id", m_sellInvoiceId).str()));
	}
}