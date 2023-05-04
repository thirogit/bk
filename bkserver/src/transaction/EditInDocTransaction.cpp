#include "stdafx.h"
#include "EditInDocTransaction.h"
#include <data/runtime/InDocOperations.h>
#include <boost/range/adaptors.hpp>
#include <boost/foreach.hpp>
#include <boost/range/algorithm.hpp>
#include "TransactionException.h"

#include <data/runtime/DocTransactionItemTuple.h>
//-------------------------------------------------------------------------------------------------

EditInDocTransaction::EditInDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId) : PlusDocTransactionSkeleton(pInterface, logger,transactionId), m_HerdId(NULL_ID), m_docId(NULL_ID)
{
}

void EditInDocTransaction::Edit(uint32_t docId)
{
	m_docId = docId;
	Do([this](SeasonSessionFacade* facade)
			  {
					IInDoc* pDoc = facade->FindInDoc(m_docId);					
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
					m_HerdId = pDoc->GetHerdId();
					m_docDetails.CopyFrom(pDoc);
			  });
}


boost::any EditInDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
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
			return m_docId;	
		case InDocOperations::Operation_GetDocDetails:
			return m_docDetails;
		case InDocOperations::Operation_GetHerdId:
			return m_HerdId;
	}
	return boost::any();
}

uint32_t EditInDocTransaction::GetId() const
{
	return GetTransactionId();
}

void EditInDocTransaction::Commit()
{
	
	Do([this](SeasonSessionFacade* facade)
		{
			OwnDocSnapshot snapshot(m_docId);
			snapshot.SetDetails(m_docDetails);
			snapshot.SetHerdId(m_HerdId);
						
			int i = 0;
			BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
			{
				snapshot.AddItem(*item);
				i++;
			}	
			facade->UpdateInDoc(snapshot);
		});	
	Clear();
}

void EditInDocTransaction::Abort()
{
	Clear();
}





