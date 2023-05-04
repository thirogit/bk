#include <stdafx.h>
#include "InvoiceTransactionSkeleton.h"
#include <arrays\DeepDelete.h>
#include "TransactionException.h"
#include <string\TextUtils.h>
#include <boost\foreach.hpp>


InvoiceTransactionSkeleton::InvoiceTransactionSkeleton(ITransactionInterface* pInterface) : m_State(TransactionState_Virgin),m_pInterface(pInterface),m_pFacade(NULL)
{
}

InvoiceTransactionSkeleton::~InvoiceTransactionSkeleton()
{
	if(m_State == TransactionState_Oppened)
	{
		Rollback();
	}	

	if(m_pFacade)
		delete m_pFacade;

	delete m_pInterface;
}

void InvoiceTransactionSkeleton::Open()
{
	if(m_State != TransactionState_Virgin)
	{
		throw TransactionException("invalid transaction state, expecting it to be in virgin state");
	}
	else
	{
		m_invoiceId = boost::any_cast<uint32_t>(m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetInvoiceId));
		size_t itemCount =  boost::any_cast<size_t>(m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetItemsCount));
		m_details.CopyFrom(boost::any_cast<InvoiceDetails>(m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetInvoiceDetails)));
		m_hent.CopyFrom(boost::any_cast<InvoiceHent>(m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetHent)));

		for(int i = 0;i < itemCount;i++)
		{
			OperationArguments args;		
			args.add(i);
			boost::any itemTupleAny = m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetItemAt,args);
			InvoiceTransactionItemTuple itemTuple = boost::any_cast<InvoiceTransactionItemTuple>(itemTupleAny);

			InvoiceItem* item = CreateItemOutOf(itemTuple);
			m_items.push_back(item);			
			
		}
		m_pFacade = new InvoiceTransactionFacadeImpl(this);
		m_State = TransactionState_Oppened;
	}
}

InvoiceItem* InvoiceTransactionSkeleton::CreateItemOutOf(const InvoiceTransactionItemTuple& itemTuple)
{
	InvoiceItem* item = new InvoiceItem(itemTuple.get<0>());
	item->SetCowId(itemTuple.get<1>());
	item->SetCowDetails(itemTuple.get<2>());
	item->SetEntry(itemTuple.get<3>());
	return item;
}

bool InvoiceTransactionSkeleton::IsUpdating() const
{
	ThrowIfNotOpen();
	return m_invoiceId != NULL_ID;
}
	
void InvoiceTransactionSkeleton::UpdateDetails(const InvoiceDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_SetInvoiceDetails,args);
	m_details.CopyFrom(details);
}

const IInvoiceHent* InvoiceTransactionSkeleton::GetHent() const
{
	ThrowIfNotOpen();
	return &m_hent;
}

void InvoiceTransactionSkeleton::SetHent(uint32_t hentId)
{
	ThrowIfNotOpen();
	m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_SetHent,OperationArguments::pack(hentId));

	boost::any hentAny = m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetHent);
	InvoiceHent hent = boost::any_cast<InvoiceHent>(hentAny);
	m_hent.CopyFrom(&hent);
}

void InvoiceTransactionSkeleton::AddDoc(uint32_t docId)
{
	ThrowIfNotOpen();
	m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_AddDoc,OperationArguments::pack(docId));

	boost::any docItemCountAny = m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetDocItemCount,OperationArguments::pack(docId));
	size_t docItemCount = boost::any_cast<size_t>(docItemCountAny);
	
	for(int i = 0;i < docItemCount;i++)
	{
		boost::any docItemAny = m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetDocItemAt,OperationArguments::pack(docId,i));
		InvoiceItem* newItem = CreateItemOutOf(boost::any_cast<InvoiceTransactionItemTuple>(docItemAny));
		m_items.push_back(newItem);
		NotifyItemAdded(newItem->GetItemKey().GetItemId());
	}	
}

void InvoiceTransactionSkeleton::UpdateCow(ItemId id,const CowDetails& details)
{
	ThrowIfNotOpen();

	InvoiceItem* item = FindItem(id);	
	m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_UpdateCow,OperationArguments::pack(item->GetItemKey(),details));
	item->SetCowDetails(details);
	NotifyItemUpdated(id);
}
void InvoiceTransactionSkeleton::UpdateEntry(ItemId id,const CowInvoiceEntryDetails& details)
{
	ThrowIfNotOpen();
	InvoiceItem* item = FindItem(id);
	m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_UpdateCowInvoiceEntry,OperationArguments::pack(item->GetItemKey().GetItemId(),details));
	NotifyItemUpdated(id);
}

void InvoiceTransactionSkeleton::DeleteDoc(uint32_t docId)
{
	ThrowIfNotOpen();	
	m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_DeleteDoc,OperationArguments::pack(docId));

	boost::multi_index::index<InvoiceItemIndex,tag_docId>::type& indexByDocId = boost::multi_index::get<tag_docId>(m_items);

	std::vector<InvoiceItem*> deletedItems;
	auto itemsRange = indexByDocId.equal_range(docId);
	BOOST_FOREACH(InvoiceItem* item,itemsRange)
	{
		deletedItems.push_back(item);				
	}
	indexByDocId.erase(docId);

	BOOST_FOREACH(InvoiceItem* item,deletedItems)
	{
		NotifyItemDeleted(item->GetItemKey().GetItemId());
		delete item;
	}
}

const InvoiceItem* InvoiceTransactionSkeleton::GetItemAt(int index)
{
	ThrowIfNotOpen();
	return m_items[index];
}

InvoiceItem* InvoiceTransactionSkeleton::FindItem(ItemId id)
{
	ThrowIfNotOpen();
		
	boost::multi_index::index<InvoiceItemIndex,tag_itemId>::type& indexByItemKey = boost::multi_index::get<tag_itemId>(m_items);

	auto itemIt = indexByItemKey.find(id);
	if(itemIt != indexByItemKey.end())
		return *itemIt;

	return NULL;
}


const InvoiceItem* InvoiceTransactionSkeleton::GetItem(ItemId id)
{
	return FindItem(id);
}

size_t InvoiceTransactionSkeleton::GetItemCount() const
{
	ThrowIfNotOpen();
	return m_items.size();
}

void InvoiceTransactionSkeleton::Commit()
{
	ThrowIfNotOpen();

	/*DocItem* pItem;
	for(std::vector<DocItem*>::iterator itemIt = m_items.begin(); itemIt != m_items.end();itemIt++)
	{
		pItem = *itemIt;
		OperationArguments updateCowArgs;
		updateCowArgs.add(pItem->GetItemId());
		updateCowArgs.add(pItem->GetCowDetails());
		m_pInterface->Execute(BuyDocOperations::Operation_UpdateCow,updateCowArgs);

		OperationArguments updateEntryArgs;
		updateEntryArgs.add(pItem->GetItemId());
		updateEntryArgs.add(pItem->GetCowEntry());
		m_pInterface->Execute(BuyDocOperations::Operation_UpdateEntry,updateEntryArgs);
	}
*/
	m_pInterface->Commit();
	m_State = TransactionState_Closed;
	//DeepDelete(m_items);
}

void InvoiceTransactionSkeleton::Rollback()
{
	ThrowIfNotOpen();
	m_pInterface->Rollback();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
	//delete m_pFacade;
	//m_pFacade = NULL;
}

const InvoiceDetails& InvoiceTransactionSkeleton::GetDetails() const
{
	return m_details;
}

void InvoiceTransactionSkeleton::ThrowIfNotOpen() const
{
	if(m_State != TransactionState_Oppened)
		throw TransactionException("transaction is not open");
}

DocScopeInvoiceTransactionFacade* InvoiceTransactionSkeleton::GetFacade()
{
	return m_pFacade;
}

size_t InvoiceTransactionSkeleton::GetDocCount()
{
	boost::any countAny = m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetDocCount);
	return boost::any_cast<size_t>(countAny);
}

uint32_t InvoiceTransactionSkeleton::GetDocAt(int index)
{
	boost::any docKeyAny = m_pInterface->Execute(BuyInvoiceDocScopeOperations::Operation_GetDocAt,OperationArguments::pack(index));
	return boost::any_cast<uint32_t>(docKeyAny);
}