#include <stdafx.h>
#include "SellDocTransaction.h"
#include <boost/any.hpp>
#include <arrays\DeepDelete.h>
#include "TransactionException.h"
#include <data\runtime\SellDocOperations.h>
#include <string\TextUtils.h>
#include <data\runtime\DocTransactionItemTuple.h>

SellDocTransactionFacade::SellDocTransactionFacade(SellDocTransaction* pTransaction) : m_pTransaction(pTransaction)
{
}

const DocItem* SellDocTransactionFacade::GetItem(ItemId id) const
{
	return m_pTransaction->GetItem(id);
}

const DocItem* SellDocTransactionFacade::GetItemAt(int index) const
{
	return m_pTransaction->GetItemAt(index);
}

count_t SellDocTransactionFacade::GetItemCount() const
{
	return m_pTransaction->GetItemCount();
}

void SellDocTransactionFacade::UpdateItemWeight(ItemId id,const NullDecimal& weight)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetWeight(weight);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void SellDocTransactionFacade::UpdateItemGroup(ItemId id,const NullInt& group)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetGroup(group);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void SellDocTransactionFacade::UpdateItemStock(ItemId id,uint32_t stockId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetStockId(stockId);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void SellDocTransactionFacade::UpdateItemClass(ItemId id,uint32_t classId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetClassId(classId);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void SellDocTransactionFacade::AddObserver(TransactionObserver* pObserver)
{
	m_pTransaction->AddObserver(pObserver);
}

void SellDocTransactionFacade::RemoveObserver(TransactionObserver* pObserver)
{
	m_pTransaction->RemoveObserver(pObserver);
}

ItemId SellDocTransactionFacade::MinusCow(uint32_t cowId)
{
	return m_pTransaction->AddCow(cowId);
}

void SellDocTransactionFacade::DeleteItem(ItemId id)
{
	m_pTransaction->DeleteCow(id);
}
//-------------------------------------------------------------------------------------------------

SellDocTransaction::SellDocTransaction(ITransactionInterface* pInterface) : m_State(TransactionState_Virgin),m_pInterface(pInterface),
																			m_docId(NULL_ID),m_pFacade(NULL),m_herdId(NULL_ID),m_hentId(NULL_ID)
{
}

SellDocTransaction::~SellDocTransaction()
{
	if(m_State == TransactionState_Oppened)
	{
		Rollback();
	}	
	delete m_pInterface;
	delete m_pFacade;
}

void SellDocTransaction::Open()
{
	if(m_State != TransactionState_Virgin)
	{
		throw TransactionException(L"invalid transaction state, expecting it to be in virgin state");
	}
	else
	{
		m_docId = boost::any_cast<uint32_t>(m_pInterface->Execute(SellDocOperations::Operation_GetDocId));
		count_t itemCount = boost::any_cast<count_t>(m_pInterface->Execute(SellDocOperations::Operation_GetItemsCount));
		m_details.CopyFrom(boost::any_cast<DocDetails>(m_pInterface->Execute(SellDocOperations::Operation_GetDocDetails)));
		m_herdId = boost::any_cast<uint32_t>(m_pInterface->Execute(SellDocOperations::Operation_GetHerd));
		m_hentId = boost::any_cast<uint32_t>(m_pInterface->Execute(SellDocOperations::Operation_GetHent));

		for(int i = 0;i < itemCount;i++)
		{
			boost::any itemAny = m_pInterface->Execute(SellDocOperations::Operation_GetItemAt,OperationArguments::pack(i));
			DocTransactionItemTuple itemTuple = boost::any_cast<DocTransactionItemTuple>(itemAny);

			DocItem* item = new DocItem(itemTuple.get<0>());
			item->SetCowId(itemTuple.get<1>());
			item->SetCowDetails(itemTuple.get<2>());
			item->SetCowEntry(itemTuple.get<3>());
			m_items.push_back(item);			
		}
		m_pFacade = new SellDocTransactionFacade(this);
		m_State = TransactionState_Oppened;
	}
}

uint32_t SellDocTransaction::GetHerdId() const
{
	return m_herdId;
}

 uint32_t SellDocTransaction::GetHent() const
 {
	 return m_hentId;
 }

bool SellDocTransaction::IsUpdating() const
{
	ThrowIfNotOpen();
	return m_docId != NULL_ID;
}
	
void SellDocTransaction::UpdateDetails(const DocDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	m_pInterface->Execute(SellDocOperations::Operation_SetDocDetails,args);
	m_details.CopyFrom(details);
}

ItemId SellDocTransaction::AddCow(uint32_t cowId)
{
	ThrowIfNotOpen();
	OperationArguments addCowArgs;
	addCowArgs.add(cowId);
	boost::any result = m_pInterface->Execute(SellDocOperations::Operation_AddCow,addCowArgs);
	ItemId newItemId = boost::any_cast<ItemId>(result);

	boost::any itemAny = m_pInterface->Execute(SellDocOperations::Operation_GetItem,OperationArguments::pack(newItemId));
	DocTransactionItemTuple itemTuple = boost::any_cast<DocTransactionItemTuple>(itemAny);

	DocItem* item = new DocItem(itemTuple.get<0>());
	item->SetCowId(itemTuple.get<1>());
	item->SetCowDetails(itemTuple.get<2>());
	item->SetCowEntry(itemTuple.get<3>());

	m_items.push_back(item);
	NotifyItemAdded(newItemId);

	return newItemId;
}


DocItem* SellDocTransaction::AcquireItem(ItemId id)
{
	const boost::multi_index::index<DocItemIndex,index_tag_itemId>::type& itemsById = boost::multi_index::get<index_tag_itemId>(m_items);

	auto itemIt = itemsById.find(id);
	if(itemIt != itemsById.end())
		return *itemIt;

	throw TransactionException(TextUtils::FormatA(L"there is not item with id %s",ItemIdText::ToString(id).c_str()));	
}


void SellDocTransaction::UpdateCowEntry(ItemId id,const CowEntryDetails& details)
{
	ThrowIfNotOpen();

	DocItem* item = AcquireItem(id);	 
	ItemId itemId = item->GetItemId();	
	m_pInterface->Execute(SellDocOperations::Operation_UpdateEntry,OperationArguments::pack(itemId,details));
	NotifyItemUpdated(itemId);
}


const DocItem* SellDocTransaction::GetItemAt(int index)
{
	ThrowIfNotOpen();
	return m_items[index];
}

const DocItem* SellDocTransaction::GetItem(ItemId id)
{
	ThrowIfNotOpen();
	return AcquireItem(id);	
}

count_t SellDocTransaction::GetItemCount() const
{
	ThrowIfNotOpen();
	return m_items.size();
}

void SellDocTransaction::Commit()
{
	ThrowIfNotOpen();
	m_pInterface->Commit();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
}

void SellDocTransaction::Rollback()
{
	ThrowIfNotOpen();
	m_pInterface->Rollback();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
	delete m_pFacade;
	m_pFacade = NULL;
}

const DocDetails& SellDocTransaction::GetDetails() const
{
	return m_details;
}

DocKey SellDocTransaction::GetDocKey() const
{
	return DocKey(DocType_Sell,m_docId);
}

void SellDocTransaction::ThrowIfNotOpen() const
{
	if(m_State != TransactionState_Oppened)
		throw TransactionException(L"transaction is not open");
}

MinusDocTransactionFacade* SellDocTransaction::GetFacade()
{
	return m_pFacade;
}

void SellDocTransaction::DeleteCow(ItemId id)
{
	OperationArguments args;
	args.add(id);
	m_pInterface->Execute(SellDocOperations::Operation_DeleteCow,args);
	NotifyItemDeleted(id);
}

void SellDocTransaction::SetHent(uint32_t hentId)
{
	OperationArguments args;
	args.add(hentId);
	m_pInterface->Execute(SellDocOperations::Operation_SetHent,args);
}

