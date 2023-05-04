#include <stdafx.h>
#include "BuyDocTransaction.h"
#include <boost/any.hpp>
#include <arrays\DeepDelete.h>
#include "TransactionException.h"
#include <string\TextUtils.h>

BuyDocTransactionFacade::BuyDocTransactionFacade(BuyDocTransaction* pTransaction) : m_pTransaction(pTransaction)
{
}

const DocItem* BuyDocTransactionFacade::GetItem(ItemId id) const
{
	return m_pTransaction->GetItem(id);
}

const DocItem* BuyDocTransactionFacade::GetItemAt(int index) const
{
	return m_pTransaction->GetItemAt(index);
}

size_t BuyDocTransactionFacade::GetItemCount() const
{
	return m_pTransaction->GetItemCount();
}

void BuyDocTransactionFacade::UpdateItemWeight(int index,const NullDecimal& weight)
{
	const DocItem* pItem = GetItemAt(index);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetWeight(weight);
	m_pTransaction->UpdateCowEntryAt(index,entry);
}

void BuyDocTransactionFacade::UpdateItemGroup(int index,const NullInt& group)
{
	const DocItem* pItem = GetItemAt(index);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetGroup(group);
	m_pTransaction->UpdateCowEntryAt(index,entry);
}

void BuyDocTransactionFacade::UpdateItemStock(int index,uint32_t stockId)
{
	const DocItem* pItem = GetItemAt(index);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetStockId(stockId);
	m_pTransaction->UpdateCowEntryAt(index,entry);
}

void BuyDocTransactionFacade::UpdateItemClass(int index,uint32_t classId)
{
	const DocItem* pItem = GetItemAt(index);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetClassId(classId);
	m_pTransaction->UpdateCowEntryAt(index,entry);
}

void BuyDocTransactionFacade::AddObserver(TransactionObserver* pObserver)
{
	m_pTransaction->AddObserver(pObserver);
}

void BuyDocTransactionFacade::RemoveObserver(TransactionObserver* pObserver)
{
	m_pTransaction->RemoveObserver(pObserver);
}

void BuyDocTransactionFacade::UpdateCow(ItemId id,const CowDetails& details)
{
}

ItemId BuyDocTransactionFacade::InCow(const CowDetails& details) const
{
	return m_pTransaction->AddCow(details);
}

void BuyDocTransactionFacade::DeleteItemAt(int index)
{
	m_pTransaction->DeleteCowAt(index);
}

void BuyDocTransactionFacade::DeleteItem(ItemId id)
{
	m_pTransaction->DeleteCow(id);
}

//-------------------------------------------------------------------------------------------------

BuyDocTransaction::BuyDocTransaction(ITransactionInterface* pInterface) : m_State(TransactionState_Virgin),m_pInterface(pInterface),m_docId(NULL_ID),m_pFacade(NULL)
{
}

BuyDocTransaction::~BuyDocTransaction()
{
	if(m_State == TransactionState_Oppened)
	{
		Rollback();
	}	
	delete m_pInterface;
}

void BuyDocTransaction::Open()
{
	if(m_State != TransactionState_Virgin)
	{
		throw TransactionException("invalid transaction state, expecting it to be in virgin state");
	}
	else
	{
		m_docId = boost::any_cast<uint32_t>(m_pInterface->Execute(BuyDocOperations::Operation_GetDocId));
		m_herdId = boost::any_cast<uint32_t>(m_pInterface->Execute(BuyDocOperations::Operation_GetHerdId));
		size_t itemCount =  boost::any_cast<size_t>(m_pInterface->Execute(BuyDocOperations::Operation_GetItemsCount));
		m_details.CopyFrom(boost::any_cast<DocDetails>(m_pInterface->Execute(BuyDocOperations::Operation_GetDocDetails)));
		for(int i = 0;i < itemCount;i++)
		{
			OperationArguments args;		
			args.add(i);
			boost::any itemIdAny = m_pInterface->Execute(BuyDocOperations::Operation_GetItemIdAt,args);
			boost::any cowDetailsAny = m_pInterface->Execute(BuyDocOperations::Operation_GetItemCowDetailsAt,args);
			boost::any cowEntryAny = m_pInterface->Execute(BuyDocOperations::Operation_GetItemCowEntryAt,args);

			DocItem* item = new DocItem(boost::any_cast<ItemId>(itemIdAny));
			item->SetCowDetails(boost::any_cast<CowDetails>(cowDetailsAny));
			item->SetCowEntry(boost::any_cast<CowEntryDetails>(cowEntryAny));
			m_items.push_back(item);
		}
		m_pFacade = new BuyDocTransactionFacade(this);
		m_State = TransactionState_Oppened;
	}
}

bool BuyDocTransaction::IsUpdating() const
{
	ThrowIfNotOpen();
	return m_docId != NULL_ID;
}
	
void BuyDocTransaction::UpdateDetails(const DocDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	m_pInterface->Execute(BuyDocOperations::Operation_SetDocDetails,args);
	m_details.CopyFrom(details);
}

ItemId BuyDocTransaction::AddCow(const CowDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	boost::any result = m_pInterface->Execute(BuyDocOperations::Operation_AddCow,args);
	ItemId newItemId = boost::any_cast<ItemId>(result);
	DocItem* newDocItem = new DocItem(newItemId);
	newDocItem->SetCowDetails(details);
	m_items.push_back(newDocItem);
	NotifyItemAdded(newItemId);
	return newItemId;
}

void BuyDocTransaction::UpdateCowAt(int index,const CowDetails& details)
{
	ThrowIfNotOpen();

	DocItem* item = m_items[index];

	OperationArguments args;
	ItemId itemId = item->GetItemId();
	args.add(itemId);
	args.add(details);
	m_pInterface->Execute(BuyDocOperations::Operation_UpdateCow,args);
	item->SetCowDetails(details);
	NotifyItemUpdated(itemId);

}

void BuyDocTransaction::UpdateCow(ItemId id,const CowDetails& details)
{
	ThrowIfNotOpen();

	auto itemIt = m_IdToItems.find(id);

	if(itemIt == m_IdToItems.end())
		throw TransactionException(TextUtils::FormatA("there is not item with id %s",ItemIdText::ToString(id).c_str()));
	
	DocItem* item = itemIt->second;

	OperationArguments args;
	ItemId itemId = item->GetItemId();
	args.add(itemId);
	args.add(details);
	m_pInterface->Execute(BuyDocOperations::Operation_UpdateCow,args);
	item->SetCowDetails(details);
	NotifyItemUpdated(itemId);
}

void BuyDocTransaction::UpdateCowEntryAt(int index,const CowEntryDetails& details)
{
	ThrowIfNotOpen();
	DocItem* item = m_items[index];
	OperationArguments args;		
	ItemId itemId = item->GetItemId();
	args.add(itemId);
	args.add(details);
	m_pInterface->Execute(BuyDocOperations::Operation_UpdateEntry,args);
	NotifyItemUpdated(itemId);
}

void BuyDocTransaction::DeleteCowAt(int index)
{
	ThrowIfNotOpen();
	OperationArguments args;		
	ItemIndex::const_iterator it = m_items.begin()+index;
	DocItem* item = *it;
	ItemId itemId = item->GetItemId();
	args.add(itemId);
	m_pInterface->Execute(BuyDocOperations::Operation_DeleteCow,args);
	m_items.erase(it);

	NotifyItemDeleted(itemId);
}

const DocItem* BuyDocTransaction::GetItemAt(int index)
{
	ThrowIfNotOpen();
	return m_items[index];
}

const DocItem* BuyDocTransaction::GetItem(ItemId id)
{
	ThrowIfNotOpen();

	const boost::multi_index::index<ItemIndex,index_tag_itemId>::type& itemsById = 
		boost::multi_index::get<index_tag_itemId>(m_items);

	auto itemIt = itemsById.find(id);
	if(itemIt != itemsById.end())
		return *itemIt;

	return NULL;
}

size_t BuyDocTransaction::GetItemCount() const
{
	ThrowIfNotOpen();
	return m_items.size();
}

void BuyDocTransaction::Commit()
{
	ThrowIfNotOpen();

	DocItem* pItem;
	for(ItemIndex::iterator itemIt = m_items.begin(); itemIt != m_items.end();itemIt++)
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

	m_pInterface->Commit();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
}

void BuyDocTransaction::Rollback()
{
	ThrowIfNotOpen();
	m_pInterface->Rollback();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
	delete m_pFacade;
	m_pFacade = NULL;
}

const DocDetails& BuyDocTransaction::GetDetails() const
{
	return m_details;
}

DocKey BuyDocTransaction::GetDocKey() const
{
	return DocKey(DocType_Buy,m_docId);
}

void BuyDocTransaction::ThrowIfNotOpen() const
{
	if(m_State != TransactionState_Oppened)
		throw TransactionException("transaction is not open");
}

PlusDocTransactionFacade* BuyDocTransaction::GetFacade()
{
	return m_pFacade;
}

uint32_t BuyDocTransaction::GetHerdId() const
{
	return m_herdId;
}

void BuyDocTransaction::DeleteCow(ItemId id)
{
	OperationArguments args;
	args.add(id);
	m_pInterface->Execute(BuyDocOperations::Operation_DeleteCow,args);
	NotifyItemDeleted(id);
}

void BuyDocTransaction::SetHent(uint32_t hentId)
{
	OperationArguments args;
	args.add(hentId);
	m_pInterface->Execute(BuyDocOperations::Operation_SetHent,args);
}
