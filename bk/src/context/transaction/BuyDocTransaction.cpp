#include <stdafx.h>
#include "BuyDocTransaction.h"
#include <boost/any.hpp>
#include <arrays\DeepDelete.h>
#include "TransactionException.h"
#include <string\TextUtils.h>
#include <data\runtime\DocTransactionItemTuple.h>
#include <CommonTypes.h>

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

count_t BuyDocTransactionFacade::GetItemCount() const
{
	return m_pTransaction->GetItemCount();
}

void BuyDocTransactionFacade::UpdateItemWeight(ItemId id,const NullDecimal& weight)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetWeight(weight);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void BuyDocTransactionFacade::UpdateItemGroup(ItemId id,const NullInt& group)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetGroup(group);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void BuyDocTransactionFacade::UpdateItemStock(ItemId id,uint32_t stockId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetStockId(stockId);
	m_pTransaction->UpdateCowEntry(id,entry);
}

void BuyDocTransactionFacade::UpdateItemClass(ItemId id,uint32_t classId)
{
	const DocItem* pItem = GetItem(id);
	CowEntryDetails entry(pItem->GetCowEntry());
	entry.SetClassId(classId);
	m_pTransaction->UpdateCowEntry(id,entry);
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
	return m_pTransaction->UpdateCow(id,details);
}

ItemId BuyDocTransactionFacade::PlusCow(const CowDetails& details)
{
	return m_pTransaction->AddCow(details);
}

void BuyDocTransactionFacade::DeleteItem(ItemId id)
{
	m_pTransaction->DeleteCow(id);
}

//-------------------------------------------------------------------------------------------------

BuyDocTransaction::BuyDocTransaction(ITransactionInterface* pInterface) : 
	m_State(TransactionState_Virgin),
	m_pInterface(pInterface),
	m_docId(NULL_ID),
	m_pFacade(NULL),
	m_hentId(NULL_ID),
	m_herdId(NULL_ID)
{
}

BuyDocTransaction::~BuyDocTransaction()
{
	if(m_State == TransactionState_Oppened)
	{
		Rollback();
	}	

	if(m_pFacade)
		delete m_pFacade;

	delete m_pInterface;
}

void BuyDocTransaction::Open()
{
	if(m_State != TransactionState_Virgin)
	{
		throw TransactionException(L"invalid transaction state, expecting it to be in virgin state");
	}
	else
	{
		m_docId = boost::any_cast<uint32_t>(m_pInterface->Execute(BuyDocOperations::Operation_GetDocId));
		m_herdId = boost::any_cast<uint32_t>(m_pInterface->Execute(BuyDocOperations::Operation_GetHerd));
		m_hentId = boost::any_cast<uint32_t>(m_pInterface->Execute(BuyDocOperations::Operation_GetHent));
		count_t itemCount =  boost::any_cast<count_t>(m_pInterface->Execute(BuyDocOperations::Operation_GetItemsCount));
		m_details.CopyFrom(boost::any_cast<DocDetails>(m_pInterface->Execute(BuyDocOperations::Operation_GetDocDetails)));
		for(int i = 0;i < itemCount;i++)
		{			 
			boost::any itemAny = m_pInterface->Execute(BuyDocOperations::Operation_GetItemAt,OperationArguments::pack(i));
			DocTransactionItemTuple itemTuple = boost::any_cast<DocTransactionItemTuple>(itemAny);

			DocItem* item = new DocItem(itemTuple.get<0>());
			item->SetCowId(itemTuple.get<1>());
			item->SetCowDetails(itemTuple.get<2>());
			item->SetCowEntry(itemTuple.get<3>());
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
	m_details.CopyFrom(details);
}

ItemId BuyDocTransaction::AddCow(const CowDetails& details)
{
	ThrowIfNotOpen();
	boost::any result = m_pInterface->Execute(BuyDocOperations::Operation_AddCow,OperationArguments::pack(details));
	ItemId newItemId = boost::any_cast<ItemId>(result);

	DocItem* newDocItem = new DocItem(newItemId);
	newDocItem->SetCowDetails(details);
	m_items.push_back(newDocItem);

	NotifyItemAdded(newItemId);
	return newItemId;
}

DocItem* BuyDocTransaction::AcquireItem(ItemId id)
{
	const boost::multi_index::index<DocItemIndex, index_tag_itemId>::type& itemsById =
		boost::multi_index::get<index_tag_itemId>(m_items);

	auto itemIt = itemsById.find(id);
	if(itemIt != itemsById.end())
		return *itemIt;

	throw TransactionException(TextUtils::FormatA(L"there is not item with id %s",ItemIdText::ToString(id).c_str()));	
}


void BuyDocTransaction::UpdateCow(ItemId id,const CowDetails& details)
{
	ThrowIfNotOpen();	
	DocItem* pItem = AcquireItem(id);
	pItem->SetCowDetails(details);
	m_pInterface->Execute(BuyDocOperations::Operation_UpdateCow,OperationArguments::pack(pItem->GetItemId(),pItem->GetCowDetails()));		
	NotifyItemUpdated(id);
}

void BuyDocTransaction::UpdateCowEntry(ItemId id,const CowEntryDetails& details)
{
	ThrowIfNotOpen();

	DocItem* pItem = AcquireItem(id);
	pItem->SetCowEntry(details);

	m_pInterface->Execute(BuyDocOperations::Operation_UpdateEntry,OperationArguments::pack(pItem->GetItemId(),pItem->GetCowEntry()));

	NotifyItemUpdated(id);
}

void BuyDocTransaction::DeleteCow(ItemId id)
{
	ThrowIfNotOpen();	 
	
	m_pInterface->Execute(BuyDocOperations::Operation_DeleteCow,OperationArguments::pack(id));

	EraseItem(id);

	NotifyItemDeleted(id);
}

const DocItem* BuyDocTransaction::GetItemAt(int index)
{
	ThrowIfNotOpen();
	return m_items[index];
}

const DocItem* BuyDocTransaction::GetItem(ItemId id)
{
	ThrowIfNotOpen();
	return AcquireItem(id);	
}

void BuyDocTransaction::EraseItem(ItemId id)
{
	boost::multi_index::index<DocItemIndex, index_tag_itemId>::type& itemsById = boost::multi_index::get<index_tag_itemId>(m_items);
	auto itemIt = itemsById.find(id);
	delete *itemIt;
	itemsById.erase(itemIt);
}


count_t BuyDocTransaction::GetItemCount() const
{
	ThrowIfNotOpen();
	return m_items.size();
}

void BuyDocTransaction::Commit()
{
	ThrowIfNotOpen();
			
	m_pInterface->Execute(BuyDocOperations::Operation_SetDocDetails,OperationArguments::pack(m_details));
	m_pInterface->Execute(BuyDocOperations::Operation_SetHent,OperationArguments::pack(m_hentId));
	
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
		throw TransactionException(L"transaction is not open");
}

PlusDocTransactionFacade* BuyDocTransaction::GetFacade()
{
	return m_pFacade;
}

uint32_t BuyDocTransaction::GetHerdId() const
{
	return m_herdId;
}

uint32_t BuyDocTransaction::GetHentId() const
{
	return m_hentId;
}

void BuyDocTransaction::SetHent(uint32_t hentId)
{
	m_hentId = hentId;	
}
