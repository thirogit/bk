#include "stdafx.h"
#include "NewSellDocTransaction.h"
#include <boost/range/adaptor/map.hpp>
#include <boost/foreach.hpp>
#include "../ClientSeasonSession.h"
#include "../SeasonSession.h"
#include <data/runtime/SellDocOperations.h>
#include "TransactionException.h"
#include <string/TextUtils.h>
#include <data/runtime/DocTransactionItemTuple.h>
#include <string/ToString.h>

//-------------------------------------------------------------------------------------------------

NewSellDocTransaction::NewSellDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId, uint32_t herdId) : DocTransactionSkeleton(pInterface, logger,transactionId), m_HerdId(herdId), m_HentId(NULL_ID)
{
}

boost::any NewSellDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
{
	switch(operation)
	{
		case SellDocOperations::Operation_SetDocDetails:
		{
			DocDetails details = args.get<DocDetails>(0);
			Log(LogLevel_Debug, L"SetDocDetails: details = %1%", ToString(details));
			SetDocDetails(details);
			break;
		}
		case SellDocOperations::Operation_AddCow:
		{
			uint32_t cowId = args.get<uint32_t>(0);
			Log(LogLevel_Debug, L"AddCow: cowId = %1%", cowId);
			ItemId itemId = AddCow(cowId);
			Log(LogLevel_Debug, L"AddCow: result = %1%", ItemIdText::ToString(itemId));
			return itemId;
		}
		case SellDocOperations::Operation_DeleteCow:
		{
			ItemId itemId = args.get<ItemId>(0);
			Log(LogLevel_Debug, L"DeleteCow: itemId = %1%", ItemIdText::ToString(itemId));
			DeleteCow(itemId);
			break;
		}
		case SellDocOperations::Operation_UpdateEntry:
		{
			ItemId itemId = args.get<ItemId>(0);
			CowEntryDetails details = args.get<CowEntryDetails>(1);
			Log(LogLevel_Debug, L"UpdateEntry: itemId = %1%, details = %2%", ItemIdText::ToString(itemId),ToString(details));
			UpdateEntry(itemId, details);
			break;
		}
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
			return NULL_ID;	

		case SellDocOperations::Operation_GetDocDetails:
			return m_docDetails;

		case SellDocOperations::Operation_GetHerd:
			return m_HerdId;
		
		case SellDocOperations::Operation_SetHent:
			m_HentId = args.get<uint32_t>(0);
			Log(LogLevel_Debug, L"SetHent: hentId = %1%", m_HentId);			
			break;
			
		case SellDocOperations::Operation_GetHent:
			return m_HentId;		
		
		default:
			BOOST_THROW_EXCEPTION(TransactionException(L"unsupported operation"));
			
			
	}
	return boost::any();
}

void NewSellDocTransaction::DeleteCow(ItemId id)
{
		DocTransactionCowItem* item = FindItem(id);	
		Do([item](SeasonSessionFacade* facade)
			{
				facade->UnlockCow(item->GetCowId());											
			});	
		return DocTransactionSkeleton::DeleteCow(id);		
}


uint32_t NewSellDocTransaction::GetId() const
{
	return GetTransactionId();
}

void NewSellDocTransaction::Commit()
{
	Log(LogLevel_Info, L"Commiting");
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

			facade->InsertSellDoc(snapshot);

			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				facade->UnlockCow(item->GetCowId());
			}
		});	
}

void NewSellDocTransaction::Abort()
{
	Log(LogLevel_Info, L"Aborting");
	Do(	[this](SeasonSessionFacade* facade)
		{										
			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				facade->UnlockCow(item->GetCowId());
			}	
											
		});
	Clear();
}

ItemId NewSellDocTransaction::AddCow(uint32_t cowId)
{
	return Eval<ItemId>([this,cowId](SeasonSessionFacade* facade) -> ItemId
						{
							facade->LockCow(cowId);
							const ICow* pCow = facade->FindCow(cowId);
							CowDetails details;
							details.CopyFrom(pCow);
							return DocTransactionSkeleton::AddCow(cowId,details);
						});	
}
