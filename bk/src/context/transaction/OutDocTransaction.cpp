#include <stdafx.h>
#include "OutDocTransaction.h"
#include <boost/any.hpp>
#include <arrays\DeepDelete.h>
#include "TransactionException.h"
#include <data\runtime\OutDocOperations.h>
#include <string\TextUtils.h>
#include <data\runtime\DocTransactionItemTuple.h>

OutDocTransactionFacade::OutDocTransactionFacade(OutDocTransaction* pTransaction) : m_pTransaction(pTransaction)
{
}

const DocItem* OutDocTransactionFacade::GetItem(ItemId id) const
{
	return m_pTransaction->GetItem(id);
}

const DocItem* OutDocTransactionFacade::GetItemAt(int index) const
{
	return m_pTransaction->GetItemAt(index);
}

count_t OutDocTransactionFacade::GetItemCount() const
{
	return m_pTransaction->GetItemCount();
}

void OutDocTransactionFacade::UpdateItemWeight(ItemId id,const NullDecimal& weight)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetWeight(weight);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void OutDocTransactionFacade::UpdateItemGroup(ItemId id,const NullInt& group)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetGroup(group);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void OutDocTransactionFacade::UpdateItemStock(ItemId id,uint32_t stockId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetStockId(stockId);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void OutDocTransactionFacade::UpdateItemClass(ItemId id,uint32_t classId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetClassId(classId);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void OutDocTransactionFacade::AddObserver(TransactionObserver* pObserver)
{
	m_pTransaction->AddObserver(pObserver);
}

void OutDocTransactionFacade::RemoveObserver(TransactionObserver* pObserver)
{
	m_pTransaction->RemoveObserver(pObserver);
}

ItemId OutDocTransactionFacade::MinusCow(uint32_t cowId)
{
	return m_pTransaction->AddCow(cowId);
}

void OutDocTransactionFacade::DeleteItem(ItemId id)
{
	m_pTransaction->DeleteCow(id);
}

//-------------------------------------------------------------------------------------------------

OutDocTransaction::OutDocTransaction(ITransactionInterface* pInterface) : m_State(TransactionState_Virgin),m_pInterface(pInterface),m_docId(NULL_ID),m_pFacade(NULL),m_herdId(NULL_ID)
{
}

OutDocTransaction::~OutDocTransaction()
{
	if(m_State == TransactionState_Oppened)
	{
		Rollback();
	}	

	if(m_pFacade)
		delete m_pFacade;

	delete m_pInterface;
}

void OutDocTransaction::Open()
{
	if(m_State != TransactionState_Virgin)
	{
		throw TransactionException(L"invalid transaction state, expecting it to be in virgin state");
	}
	else
	{
		m_docId = boost::any_cast<uint32_t>(m_pInterface->Execute(OutDocOperations::Operation_GetDocId));
		m_herdId = boost::any_cast<uint32_t>(m_pInterface->Execute(OutDocOperations::Operation_GetHerd));
		count_t itemCount = boost::any_cast<count_t>(m_pInterface->Execute(OutDocOperations::Operation_GetItemsCount));
		m_details.CopyFrom(boost::any_cast<DocDetails>(m_pInterface->Execute(OutDocOperations::Operation_GetDocDetails)));

		for(int i = 0;i < itemCount;i++)
		{			
			boost::any itemAny = m_pInterface->Execute(OutDocOperations::Operation_GetItemAt,OperationArguments::pack(i));
			DocTransactionItemTuple itemTuple = boost::any_cast<DocTransactionItemTuple>(itemAny);

			DocItem* item = new DocItem(itemTuple.get<0>());
			item->SetCowId(itemTuple.get<1>());
			item->SetCowDetails(itemTuple.get<2>());
			item->SetCowEntry(itemTuple.get<3>());

			m_items.push_back(item);		
		}
		m_pFacade = new OutDocTransactionFacade(this);
		m_State = TransactionState_Oppened;
	}
}

bool OutDocTransaction::IsUpdating() const
{
	ThrowIfNotOpen();
	return m_docId != NULL_ID;
}
	
void OutDocTransaction::UpdateDetails(const DocDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	m_pInterface->Execute(OutDocOperations::Operation_SetDocDetails,args);
	m_details.CopyFrom(details);
}

ItemId OutDocTransaction::AddCow(uint32_t cowId)
{
	ThrowIfNotOpen();
	boost::any addResult = m_pInterface->Execute(OutDocOperations::Operation_AddCow,OperationArguments::pack(cowId));
	ItemId newItemId = boost::any_cast<ItemId>(addResult);
	boost::any itemAny = m_pInterface->Execute(OutDocOperations::Operation_GetItem,OperationArguments::pack(newItemId));
	DocTransactionItemTuple itemTuple = boost::any_cast<DocTransactionItemTuple>(itemAny);

	DocItem* item = new DocItem(itemTuple.get<0>());
	item->SetCowId(itemTuple.get<1>());
	item->SetCowDetails(itemTuple.get<2>());
	item->SetCowEntry(itemTuple.get<3>());
	
	m_items.push_back(item);
	NotifyItemAdded(newItemId);
	return newItemId;
}

void OutDocTransaction::UpdateCowEntry(ItemId id,const CowEntryDetails& details)
{
	ThrowIfNotOpen();
	DocItem* item = AcquireItem(id);	
	ItemId itemId = item->GetItemId();	
	m_pInterface->Execute(OutDocOperations::Operation_UpdateEntry,OperationArguments::pack(itemId,details));
	NotifyItemUpdated(itemId);
}

const DocItem* OutDocTransaction::GetItemAt(int index)
{
	ThrowIfNotOpen();
	return m_items[index];
}

DocItem* OutDocTransaction::FindItem(ItemId id)
{
	const boost::multi_index::index<DocItemIndex, index_tag_itemId>::type& itemsById =
		boost::multi_index::get<index_tag_itemId>(m_items);

	auto itemIt = itemsById.find(id);
	if(itemIt != itemsById.end())
		return *itemIt;

	return NULL;
}

const DocItem* OutDocTransaction::GetItem(ItemId id)
{
	return FindItem(id);
}

count_t OutDocTransaction::GetItemCount() const
{
	ThrowIfNotOpen();
	return m_items.size();
}

void OutDocTransaction::Commit()
{
	ThrowIfNotOpen();
	m_pInterface->Commit();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
}

void OutDocTransaction::Rollback()
{
	ThrowIfNotOpen();
	m_pInterface->Rollback();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
	delete m_pFacade;
	m_pFacade = NULL;
}

const DocDetails& OutDocTransaction::GetDetails() const
{
	return m_details;
}

DocKey OutDocTransaction::GetDocKey() const
{
	return DocKey(DocType_Out,m_docId);
}

void OutDocTransaction::ThrowIfNotOpen() const
{
	if(m_State != TransactionState_Oppened)
		throw TransactionException(L"transaction is not open");
}

MinusDocTransactionFacade* OutDocTransaction::GetFacade()
{
	return m_pFacade;
}

void OutDocTransaction::DeleteCow(ItemId id)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(id);
	m_pInterface->Execute(OutDocOperations::Operation_DeleteCow,args);
	NotifyItemDeleted(id);
}

DocItem* OutDocTransaction::AcquireItem(ItemId id)
{
	DocItem* item = FindItem(id);
	if(!item)
		throw TransactionException(TextUtils::FormatA(L"there is not item with id %s",ItemIdText::ToString(id).c_str()));

	return item;
}

uint32_t OutDocTransaction::GetHerdId() const
{
	return m_herdId;
}