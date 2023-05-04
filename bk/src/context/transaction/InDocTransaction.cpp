#include <stdafx.h>
#include "InDocTransaction.h"
#include <boost/any.hpp>
#include <arrays\DeepDelete.h>
#include "TransactionException.h"
#include <data\runtime\DocTransactionItemTuple.h>
#include <string\TextUtils.h>

InDocTransactionFacade::InDocTransactionFacade(InDocTransaction* pTransaction) : m_pTransaction(pTransaction)
{
}

const DocItem* InDocTransactionFacade::GetItem(ItemId id) const
{
	return m_pTransaction->GetItem(id);
}

const DocItem* InDocTransactionFacade::GetItemAt(int index) const
{
	return m_pTransaction->GetItemAt(index);
}

count_t InDocTransactionFacade::GetItemCount() const
{
	return m_pTransaction->GetItemCount();
}

void InDocTransactionFacade::UpdateItemWeight(ItemId id,const NullDecimal& weight)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetWeight(weight);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void InDocTransactionFacade::UpdateItemGroup(ItemId id,const NullInt& group)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetGroup(group);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void InDocTransactionFacade::UpdateItemStock(ItemId id,uint32_t stockId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetStockId(stockId);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void InDocTransactionFacade::UpdateItemClass(ItemId id,uint32_t classId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetClassId(classId);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void InDocTransactionFacade::AddObserver(TransactionObserver* pObserver)
{
	m_pTransaction->AddObserver(pObserver);
}

void InDocTransactionFacade::RemoveObserver(TransactionObserver* pObserver)
{
	m_pTransaction->RemoveObserver(pObserver);
}

ItemId InDocTransactionFacade::PlusCow(const CowDetails& details)
{
	return m_pTransaction->AddCow(details);
}

void InDocTransactionFacade::UpdateCow(ItemId id,const CowDetails& details)
{
	m_pTransaction->UpdateCow(id,details);
}

void InDocTransactionFacade::DeleteItem(ItemId id)
{
	m_pTransaction->DeleteCow(id);
}

//-------------------------------------------------------------------------------------------------

InDocTransaction::InDocTransaction(ITransactionInterface* pInterface) : 
	m_State(TransactionState_Virgin),
	m_pInterface(pInterface),
	m_docId(NULL_ID),
	m_herdId(NULL_ID)
{
	m_pFacade = new InDocTransactionFacade(this);
}

InDocTransaction::~InDocTransaction()
{
	if(m_State == TransactionState_Oppened)
	{
		Rollback();
	}	

	delete m_pFacade;
	delete m_pInterface;
}

void InDocTransaction::Open()
{
	if(m_State != TransactionState_Virgin)
	{
		throw TransactionException(L"invalid transaction state, expecting it to be in virgin state");
	}
	else
	{
		m_docId = boost::any_cast<uint32_t>(m_pInterface->Execute(InDocOperations::Operation_GetDocId));
		m_herdId = boost::any_cast<uint32_t>(m_pInterface->Execute(InDocOperations::Operation_GetHerdId));
		count_t itemCount =  boost::any_cast<count_t>(m_pInterface->Execute(InDocOperations::Operation_GetItemsCount));
		m_details.CopyFrom(boost::any_cast<DocDetails>(m_pInterface->Execute(InDocOperations::Operation_GetDocDetails)));
		for(int i = 0;i < itemCount;i++)
		{
			OperationArguments args;		
			args.add(i);
			boost::any itemAny = m_pInterface->Execute(InDocOperations::Operation_GetItemAt,args);
			
			DocTransactionItemTuple itemTuple = boost::any_cast<DocTransactionItemTuple>(itemAny);

			DocItem* item = new DocItem(itemTuple.get<0>());
			item->SetCowId(itemTuple.get<1>());
			item->SetCowDetails(itemTuple.get<2>());
			item->SetCowEntry(itemTuple.get<3>());
			
			m_items.push_back(item);			
		}		
		m_State = TransactionState_Oppened;
	}
}

bool InDocTransaction::IsUpdating() const
{
	ThrowIfNotOpen();
	return m_docId != NULL_ID;
}
	
void InDocTransaction::UpdateDetails(const DocDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	m_pInterface->Execute(InDocOperations::Operation_SetDocDetails,args);
	m_details.CopyFrom(details);
}

ItemId InDocTransaction::AddCow(const CowDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	boost::any result = m_pInterface->Execute(InDocOperations::Operation_AddCow,args);
	ItemId newItemId = boost::any_cast<ItemId>(result);
	DocItem* newDocItem = new DocItem(newItemId);
	newDocItem->SetCowDetails(details);
	m_items.push_back(newDocItem);
	NotifyItemAdded(newItemId);
	return newItemId;
}

void InDocTransaction::UpdateCow(ItemId id,const CowDetails& details)
{
	ThrowIfNotOpen();
	DocItem* item = AcquireItem(id);	
	ItemId itemId = item->GetItemId();	
	m_pInterface->Execute(InDocOperations::Operation_UpdateCow,OperationArguments::pack(itemId,details));
	item->SetCowDetails(details);
	NotifyItemUpdated(itemId);

}

void InDocTransaction::UpdateCowEntry(ItemId id,const CowEntryDetails& details)
{
	ThrowIfNotOpen();
	DocItem* item = AcquireItem(id);	 		
	ItemId itemId = item->GetItemId();	
	m_pInterface->Execute(InDocOperations::Operation_UpdateEntry,OperationArguments::pack(itemId,details));
	item->SetCowEntry(details);
	NotifyItemUpdated(itemId);
}

const DocItem* InDocTransaction::GetItemAt(int index)
{
	ThrowIfNotOpen();
	return m_items[index];
}

const DocItem* InDocTransaction::GetItem(ItemId id)
{
	ThrowIfNotOpen();

	const boost::multi_index::index<DocItemIndex, index_tag_itemId>::type& itemsById =
		boost::multi_index::get<index_tag_itemId>(m_items);

	auto itemIt = itemsById.find(id);
	if(itemIt != itemsById.end())
		return *itemIt;

	return NULL;
}

count_t InDocTransaction::GetItemCount() const
{
	ThrowIfNotOpen();
	return m_items.size();
}

void InDocTransaction::Commit()
{
	ThrowIfNotOpen();

	//DocItem* pItem;
	//for(ItemIndex::iterator itemIt = m_items.begin(); itemIt != m_items.end();itemIt++)
	//{
	//	pItem = *itemIt;
	//	OperationArguments updateCowArgs;
	//	updateCowArgs.add(pItem->GetItemId());
	//	updateCowArgs.add(pItem->GetCowDetails());
	//	m_pInterface->Execute(InDocOperations::Operation_UpdateCow,updateCowArgs);

	//	OperationArguments updateEntryArgs;
	//	updateEntryArgs.add(pItem->GetItemId());
	//	updateEntryArgs.add(pItem->GetCowEntry());
	//	m_pInterface->Execute(InDocOperations::Operation_UpdateEntry,updateEntryArgs);
	//}

	m_pInterface->Commit();
	m_State = TransactionState_Closed;
	
	DeepDelete(m_items);
	m_items.clear();
}

void InDocTransaction::Rollback()
{
	ThrowIfNotOpen();
	m_pInterface->Rollback();
	m_State = TransactionState_Closed;
	
	DeepDelete(m_items);
	m_items.clear();
}

const DocDetails& InDocTransaction::GetDetails() const
{
	return m_details;
}

DocKey InDocTransaction::GetDocKey() const
{
	return DocKey(DocType_In,m_docId);
}

void InDocTransaction::ThrowIfNotOpen() const
{
	if(m_State != TransactionState_Oppened)
		throw TransactionException(L"transaction is not open");
}

PlusDocTransactionFacade* InDocTransaction::GetFacade()
{
	return m_pFacade;
}

DocItem* InDocTransaction::AcquireItem(ItemId id)
{
	const boost::multi_index::index<DocItemIndex, index_tag_itemId>::type& itemsById =
		boost::multi_index::get<index_tag_itemId>(m_items);

	auto itemIt = itemsById.find(id);
	if(itemIt != itemsById.end())
		return *itemIt;

	throw TransactionException(TextUtils::FormatA(L"there is not item with id %s",ItemIdText::ToString(id).c_str()));
}


void InDocTransaction::EraseItem(ItemId id)
{
	boost::multi_index::index<DocItemIndex, index_tag_itemId>::type& itemsById = boost::multi_index::get<index_tag_itemId>(m_items);
	auto itemIt = itemsById.find(id);
	delete *itemIt;
	itemsById.erase(itemIt);
}

void InDocTransaction::DeleteCow(ItemId id)
{
	ThrowIfNotOpen();
	
	m_pInterface->Execute(InDocOperations::Operation_DeleteCow,OperationArguments::pack(id));

	EraseItem(id);

	NotifyItemDeleted(id);
}

uint32_t InDocTransaction::GetHerdId() const
{
	return m_herdId;
}

ItemId InDocTransaction::GetItemWithCowNo(const CowNo& cowNo) const
{
	if (cowNo.IsNull()) {
		return ItemId();
	}

	auto it = m_items.cbegin();
	while (it != m_items.cend()) {
		if ((*it)->GetCowDetails().GetCowNo() == cowNo) {
			return (*it)->GetItemId();
		}
		it++;
	}
	return ItemId();
}