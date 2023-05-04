#include <stdafx.h>
#include "MoveDocTransaction.h"
#include <boost/any.hpp>
#include <arrays\DeepDelete.h>
#include "TransactionException.h"
#include <string\TextUtils.h>
#include <data\runtime\DocTransactionItemTuple.h>

MoveDocTransactionFacade::MoveDocTransactionFacade(MoveDocTransaction* pTransaction) : m_pTransaction(pTransaction)
{
}

const DocItem* MoveDocTransactionFacade::GetItem(ItemId id) const
{
	return m_pTransaction->GetItem(id);
}

const DocItem* MoveDocTransactionFacade::GetItemAt(int index) const
{
	return m_pTransaction->GetItemAt(index);
}

count_t MoveDocTransactionFacade::GetItemCount() const
{
	return m_pTransaction->GetItemCount();
}

void MoveDocTransactionFacade::UpdateItemWeight(ItemId id,const NullDecimal& weight)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetWeight(weight);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void MoveDocTransactionFacade::UpdateItemGroup(ItemId id,const NullInt& group)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetGroup(group);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void MoveDocTransactionFacade::UpdateItemStock(ItemId id,uint32_t stockId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetStockId(stockId);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void MoveDocTransactionFacade::UpdateItemClass(ItemId id,uint32_t classId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetClassId(classId);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void MoveDocTransactionFacade::AddObserver(TransactionObserver* pObserver)
{
	m_pTransaction->AddObserver(pObserver);
}

void MoveDocTransactionFacade::RemoveObserver(TransactionObserver* pObserver)
{
	m_pTransaction->RemoveObserver(pObserver);
}

void MoveDocTransactionFacade::DeleteItem(ItemId id)
{
	m_pTransaction->DeleteCow(id);
}

ItemId MoveDocTransactionFacade::MinusCow(uint32_t cowId)
{
	return m_pTransaction->AddCow(cowId);
}

//-------------------------------------------------------------------------------------------------

MoveDocTransaction::MoveDocTransaction(ITransactionInterface* pInterface) : m_State(TransactionState_Virgin),m_pInterface(pInterface),m_docId(NULL_ID),m_pFacade(NULL)
{
}

MoveDocTransaction::~MoveDocTransaction()
{
	if(m_State == TransactionState_Oppened)
	{
		Rollback();
	}	

	if(m_pFacade)
		delete m_pFacade;

	delete m_pInterface;
}

void MoveDocTransaction::Open()
{
	if(m_State != TransactionState_Virgin)
	{
		throw TransactionException(L"invalid transaction state, expecting it to be in virgin state");
	}
	else
	{
		m_docId = boost::any_cast<uint32_t>(m_pInterface->Execute(MoveDocOperations::Operation_GetDocId));

		m_srcHerdId = boost::any_cast<uint32_t>(m_pInterface->Execute(MoveDocOperations::Operation_GetSrcHerdId));
		m_dstHerdId = boost::any_cast<uint32_t>(m_pInterface->Execute(MoveDocOperations::Operation_GetDstHerdId));

		count_t itemCount = boost::any_cast<count_t>(m_pInterface->Execute(MoveDocOperations::Operation_GetItemsCount));
		m_details.CopyFrom(boost::any_cast<DocDetails>(m_pInterface->Execute(MoveDocOperations::Operation_GetDocDetails)));
		for(int i = 0;i < itemCount;i++)
		{
			OperationArguments args;		
			args.add(i);
			boost::any itemAny = m_pInterface->Execute(MoveDocOperations::Operation_GetItemAt,args);
			DocTransactionItemTuple itemTuple = boost::any_cast<DocTransactionItemTuple>(itemAny);
			DocItem* item = new DocItem(itemTuple.get<0>());
			item->SetCowId(itemTuple.get<1>());
			item->SetCowDetails(itemTuple.get<2>());
			item->SetCowEntry(itemTuple.get<3>());
			m_items.push_back(item);
		}
		m_pFacade = new MoveDocTransactionFacade(this);
		m_State = TransactionState_Oppened;
	}
}

bool MoveDocTransaction::IsUpdating() const
{
	ThrowIfNotOpen();
	return m_docId != NULL_ID;
}
	
void MoveDocTransaction::UpdateDetails(const DocDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	m_pInterface->Execute(MoveDocOperations::Operation_SetDocDetails,args);
	m_details.CopyFrom(details);
}

ItemId MoveDocTransaction::AddCow(uint32_t cowId)
{
	ThrowIfNotOpen();
	OperationArguments addCowArgs;
	
	boost::any addCowResult = m_pInterface->Execute(MoveDocOperations::Operation_AddCow,OperationArguments::pack(cowId));

	ItemId newItemId = boost::any_cast<ItemId>(addCowResult);
	DocItem* newDocItem = new DocItem(newItemId);
	 
	boost::any itemAny = m_pInterface->Execute(MoveDocOperations::Operation_GetItem,OperationArguments::pack(newItemId));

	DocTransactionItemTuple itemTuple = boost::any_cast<DocTransactionItemTuple>(itemAny);
	newDocItem->SetCowId(itemTuple.get<1>());
	newDocItem->SetCowDetails(itemTuple.get<2>());	
	
	m_items.push_back(newDocItem);
	NotifyItemAdded(newItemId);

	return newItemId;
}

DocItem* MoveDocTransaction::AcquireItem(ItemId id)
{
	const boost::multi_index::index<DocItemIndex, index_tag_itemId>::type& itemsById =
		boost::multi_index::get<index_tag_itemId>(m_items);

	auto itemIt = itemsById.find(id);
	if(itemIt != itemsById.end())
		return *itemIt;

	throw TransactionException(TextUtils::FormatA(L"there is not item with id %s",ItemIdText::ToString(id).c_str()));
}

void MoveDocTransaction::UpdateCowEntry(ItemId id,const CowEntryDetails& details)
{
	ThrowIfNotOpen();
	DocItem* item = AcquireItem(id);
	ItemId itemId = item->GetItemId();
	m_pInterface->Execute(MoveDocOperations::Operation_UpdateEntry,OperationArguments::pack(itemId,details));
	item->SetCowEntry(details);
	NotifyItemUpdated(itemId);	
}

const DocItem* MoveDocTransaction::GetItemAt(int index)
{
	ThrowIfNotOpen();
	return m_items[index];
}

const DocItem* MoveDocTransaction::GetItem(ItemId id)
{
	ThrowIfNotOpen();
	return AcquireItem(id);
}

count_t MoveDocTransaction::GetItemCount() const
{
	ThrowIfNotOpen();
	return m_items.size();
}

void MoveDocTransaction::Commit()
{
	ThrowIfNotOpen();	
	m_pInterface->Commit();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
}

void MoveDocTransaction::Rollback()
{
	ThrowIfNotOpen();
	m_pInterface->Rollback();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
	delete m_pFacade;
	m_pFacade = NULL;
}

const DocDetails& MoveDocTransaction::GetDetails() const
{
	return m_details;
}

DocKey MoveDocTransaction::GetDocKey() const
{
	return DocKey(DocType_Move,m_docId);
}

void MoveDocTransaction::ThrowIfNotOpen() const
{
	if(m_State != TransactionState_Oppened)
		throw TransactionException(L"transaction is not open");
}

MinusDocTransactionFacade* MoveDocTransaction::GetFacade()
{
	return m_pFacade;
}

uint32_t MoveDocTransaction::GetSrcHerdId()
{
	return m_srcHerdId;
}

uint32_t MoveDocTransaction::GetDstHerdId()
{
	return m_dstHerdId;
}

void MoveDocTransaction::EraseItem(ItemId id)
{
	boost::multi_index::index<DocItemIndex, index_tag_itemId>::type& itemsById = boost::multi_index::get<index_tag_itemId>(m_items);
	auto itemIt = itemsById.find(id);
	delete *itemIt;
	itemsById.erase(itemIt);
}

void MoveDocTransaction::DeleteCow(ItemId id)
{
	ThrowIfNotOpen();
	 
	m_pInterface->Execute(MoveDocOperations::Operation_DeleteCow,OperationArguments::pack(id));

	EraseItem(id);

	NotifyItemDeleted(id);
}