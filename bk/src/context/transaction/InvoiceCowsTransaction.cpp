#include <stdafx.h>
#include "InvoiceCowsTransaction.h"
#include <boost/any.hpp>
#include <arrays\DeepDelete.h>
#include "TransactionException.h"
#include <string\TextUtils.h>
#include <data\runtime\InvoiceCowsScopeOperations.h>
#include <boost\foreach.hpp>

InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::InvoiceTransactionFacadeImpl(InvoiceCowsTransaction* pOwner)
{
	m_pOwner = pOwner;
}

const InvoiceDetails& InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetDetails() const
{
	return m_pOwner->GetDetails();
}

InvoiceType InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetInvoiceType() const
{
	return  m_pOwner->GetInvoiceType();
}

void InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::UpdateDetails(const InvoiceDetails& details)
{
	m_pOwner->UpdateDetails(details);
}

void InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::UpdateCow(ItemId id,const CowDetails& details)
{
	m_pOwner->UpdateCow(id,details);
}

void InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::UpdateEntry(ItemId id,const CowInvoiceEntryDetails& details)
{
	m_pOwner->UpdateEntry(id,details);
}

const InvoiceTransactionItem* InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetItemAt(int index)
{
	return m_pOwner->GetItemAt(index);
}

const InvoiceTransactionItem* InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetItem(ItemId id)
{
	return m_pOwner->GetItem(id);
}

count_t InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetItemCount() const
{
	return m_pOwner->GetItemCount();
}

void InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::AddObserver(InvoiceTransactionObserver* pObserver)
{
	m_pOwner->AddObserver(pObserver);
}

void InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::RemoveObserver(InvoiceTransactionObserver* pObserver)
{
	m_pOwner->RemoveObserver(pObserver);
}

ItemId InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::AddCow(uint32_t cowId)
{
	return m_pOwner->AddCow(cowId);
}

void InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::DeleteCow(uint32_t cowId)
{
	const InvoiceTransactionItem* pItem = m_pOwner->FindItem(cowId);
	if(pItem)
		m_pOwner->DeleteItem(pItem->GetItemKey().GetItemId());	
}

const InvoiceTransactionItem* InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetItem(uint32_t cowId)
{
	return m_pOwner->FindItem(cowId);
}

uint32_t InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetDocId() const
{
	return m_pOwner->GetDocId();
}

const InvoiceTransactionDeductionItem* InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetDeductionAt(int index)
{
	return m_pOwner->m_deductions.item_at(index);
}

const InvoiceTransactionDeductionItem* InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetDeduction(ItemId id)
{
	return m_pOwner->m_deductions.find_item(id);
}

count_t InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::GetDeductionCount() const
{
	return m_pOwner->m_deductions.size();
}

void InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::UpdateDeduction(ItemId id, const InvoiceDeduction& deduction)
{
	m_pOwner->UpdateDeduction(id, deduction);
}

ItemId InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::AddDeduction(const InvoiceDeduction& deduction)
{
	return m_pOwner->AddDeduction(deduction);
}

void InvoiceCowsTransaction::InvoiceTransactionFacadeImpl::RemoveDeduction(ItemId id)
{
	m_pOwner->RemoveDeduction(id);
}
//-------------------------------------------------------------------------------------------------

InvoiceCowsTransaction::InvoiceCowsTransaction(ITransactionInterface* pInterface) : 
	m_State(TransactionState_Virgin),
	m_pInterface(pInterface),
	m_pFacade(NULL),
	m_docId(NULL_ID)
{
}

InvoiceCowsTransaction::~InvoiceCowsTransaction()
{
	if(m_State == TransactionState_Oppened)
	{
		Rollback();
	}	

	if(m_pFacade)
		delete m_pFacade;

	delete m_pInterface;
}

void InvoiceCowsTransaction::Open()
{
	if(m_State != TransactionState_Virgin)
	{
		throw TransactionException(L"invalid transaction state, expecting it to be in virgin state");
	}
	else
	{
		m_invoiceId = boost::any_cast<uint32_t>(m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetInvoiceId));
		m_invoiceNo = boost::any_cast<uint32_t>(m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetInvoiceNo));
		count_t itemCount =  boost::any_cast<count_t>(m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetItemsCount));
		m_details.CopyFrom(boost::any_cast<InvoiceDetails>(m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetInvoiceDetails)));
		m_hent.CopyFrom(boost::any_cast<InvoiceHent>(m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetHent)));
		m_docId = boost::any_cast<uint32_t>(m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetDoc));
		count_t deductionCount = boost::any_cast<count_t>(m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetDeductionsCount));
		m_invoiceType = boost::any_cast<InvoiceType>(m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetInvoiceType));

		for(int i = 0;i < itemCount;i++)
		{
			OperationArguments args;		
			args.add(i);
			boost::any itemTupleAny = m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetItemAt,args);
			InvoiceTransactionItemTuple itemTuple = boost::any_cast<InvoiceTransactionItemTuple>(itemTupleAny);

			InvoiceTransactionItem* item = CreateItemOutOf(itemTuple);
			m_items.push_back(item);			
			
		}

		for (int i = 0; i < deductionCount; i++)
		{
			OperationArguments args;
			args.add(i);
			boost::any deductionTupleAny = m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetDeductionAt, args);
			DeductionItemTuple deductionTuple = boost::any_cast<DeductionItemTuple>(deductionTupleAny);

			InvoiceTransactionDeductionItem* deductionItem = CreateDeductionItemOutOf(deductionTuple);
			m_deductions.add_item(deductionItem);

		}

		m_pFacade = new InvoiceTransactionFacadeImpl(this);
		m_State = TransactionState_Oppened;
	}
}

InvoiceTransactionItem* InvoiceCowsTransaction::CreateItemOutOf(const InvoiceTransactionItemTuple& itemTuple)
{
	InvoiceTransactionItem* item = new InvoiceTransactionItem(itemTuple.get<0>());
	item->SetCowId(itemTuple.get<1>());
	item->SetCowDetails(itemTuple.get<2>());
	item->SetEntry(itemTuple.get<3>());
	return item;
}

InvoiceTransactionDeductionItem* InvoiceCowsTransaction::CreateDeductionItemOutOf(const DeductionItemTuple& itemTuple)
{
	InvoiceTransactionDeductionItem* deductionItem = new InvoiceTransactionDeductionItem(itemTuple.get<0>());
	InvoiceDeduction deduction = itemTuple.get<1>();
	deductionItem->SetDetails(deduction.GetDetails());
	deductionItem->SetEnabled(deduction.GetIsEnabled());

	return deductionItem;
}

bool InvoiceCowsTransaction::IsUpdating() const
{
	ThrowIfNotOpen();
	return m_invoiceId != NULL_ID;
}
	
void InvoiceCowsTransaction::UpdateDetails(const InvoiceDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_SetInvoiceDetails,args);
	m_details.CopyFrom(details);
}

const InvoiceHent& InvoiceCowsTransaction::GetHent() const
{
	ThrowIfNotOpen();
	return m_hent;
}

void InvoiceCowsTransaction::SetHent(const InvoiceHent& hent)
{
	ThrowIfNotOpen();
	m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_SetHent,OperationArguments::pack(hent));	
	m_hent.CopyFrom(hent);
}

void InvoiceCowsTransaction::UpdateCow(ItemId id,const CowDetails& details)
{
	ThrowIfNotOpen();

	InvoiceTransactionItem* item = FindItem(id);
	m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_UpdateCow,OperationArguments::pack(item->GetItemKey(),details));
	item->SetCowDetails(details);
	NotifyItemUpdated(id);
}
void InvoiceCowsTransaction::UpdateEntry(ItemId id,const CowInvoiceEntryDetails& details)
{
	ThrowIfNotOpen();
	InvoiceTransactionItem* item = FindItem(id);
	m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_UpdateCowInvoiceEntry,OperationArguments::pack(item->GetItemKey().GetItemId(),details));
	NotifyItemUpdated(id);
}


const InvoiceTransactionItem* InvoiceCowsTransaction::GetItemAt(int index)
{
	ThrowIfNotOpen();
	return m_items.get_at(index);
}

InvoiceTransactionItem* InvoiceCowsTransaction::FindItem(ItemId id)
{
	ThrowIfNotOpen();
	return m_items.find_item(id);
}


const InvoiceTransactionItem* InvoiceCowsTransaction::GetItem(ItemId id)
{
	return FindItem(id);
}

count_t InvoiceCowsTransaction::GetItemCount() const
{
	ThrowIfNotOpen();
	return m_items.size();
}

void InvoiceCowsTransaction::Commit()
{
	ThrowIfNotOpen();	
	m_pInterface->Commit();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
}

void InvoiceCowsTransaction::Rollback()
{
	ThrowIfNotOpen();
	m_pInterface->Rollback();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
}

const InvoiceDetails& InvoiceCowsTransaction::GetDetails() const
{
	return m_details;
}

void InvoiceCowsTransaction::ThrowIfNotOpen() const
{
	if(m_State != TransactionState_Oppened)
		throw TransactionException(L"transaction is not open");
}

CowsScopeInvoiceTransactionFacade* InvoiceCowsTransaction::GetFacade()
{
	return m_pFacade;
}

ItemId InvoiceCowsTransaction::AddCow(uint32_t cowId)
{	
	boost::any itemAny = m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_AddCow,OperationArguments::pack(cowId));	
	ItemId id = boost::any_cast<ItemId>(itemAny);
	boost::any itemTupleAny = m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetItem,OperationArguments::pack(id));
	InvoiceTransactionItemTuple itemTuple = boost::any_cast<InvoiceTransactionItemTuple>(itemTupleAny);
	InvoiceTransactionItem* item = CreateItemOutOf(itemTuple);
	m_items.push_back(item);			
	NotifyItemAdded(id);
	return id;
}

const InvoiceTransactionItem* InvoiceCowsTransaction::FindItem(uint32_t cowId)
{
	return m_items.find_item(cowId);
}

void InvoiceCowsTransaction::DeleteItem(ItemId id)
{
	m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_DeleteItem,OperationArguments::pack(id));
	InvoiceTransactionItem* pItem = m_items.delete_item(id);
	if(pItem)
	{			
		NotifyItemDeleted(id);
		delete pItem;
	}
}

uint32_t InvoiceCowsTransaction::GetDocId() const
{
	return m_docId;
}

void InvoiceCowsTransaction::SetDocId(uint32_t docId)
{
	m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_SetDoc,OperationArguments::pack(docId));
	m_docId = docId;
}

InvoiceType InvoiceCowsTransaction::GetInvoiceType() const
{
	return m_invoiceType;
}

void InvoiceCowsTransaction::SetInvoiceType(InvoiceType type)
{
	m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_SetInvoiceType, OperationArguments::pack(type));
	m_invoiceType = type;
}

uint32_t InvoiceCowsTransaction::GetInvoiceNo() const
{
	return m_invoiceNo;
}

InvoiceTransactionDeductionItem* InvoiceCowsTransaction::AcquireDeduction(ItemId id)
{
	InvoiceTransactionDeductionItem* deduction = m_deductions.find_item(id);
	if (!deduction)
		throw TransactionException(L"deduction_item_not_found");
	return deduction;
}

size_t InvoiceCowsTransaction::GetDeductionCount() const
{
	return m_deductions.size();
}

const InvoiceTransactionDeductionItem* InvoiceCowsTransaction::GetDeduction(ItemId id)
{
	return m_deductions.find_item(id);
}

const InvoiceTransactionDeductionItem* InvoiceCowsTransaction::GetDeductionAt(int index)
{
	return m_deductions.item_at(index);
}

void InvoiceCowsTransaction::UpdateDeduction(ItemId id, const InvoiceDeduction& deduction)
{
	InvoiceTransactionDeductionItem* deductionItem = AcquireDeduction(id);
	DeductionItemTuple deductionTuple(id, deduction);
	m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_UpdateDeduction, OperationArguments::pack(deductionTuple));

	deductionItem->SetDeduction(deduction);
	NotifyDeductionUpdated(id);
}

ItemId InvoiceCowsTransaction::AddDeduction(const InvoiceDeduction& deduction)
{
	boost::any addResultAny = m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_AddDeduction, OperationArguments::pack(deduction));
	ItemId itemId = boost::any_cast<ItemId>(addResultAny);

	boost::any getResultAny = m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_GetDeduction, OperationArguments::pack(itemId));
	DeductionItemTuple deductionTuple = boost::any_cast<DeductionItemTuple>(getResultAny);

	InvoiceTransactionDeductionItem* item = CreateDeductionItemOutOf(deductionTuple);
	m_deductions.add_item(item);
	NotifyDeductionAdded(itemId);

	return itemId;
}

void InvoiceCowsTransaction::RemoveDeduction(ItemId id)
{
	boost::any getResultAny = m_pInterface->Execute(InvoiceCowsScopeOperations::Operation_RemoveDeduction, OperationArguments::pack(id));
	InvoiceTransactionDeductionItem* removedItem = m_deductions.remove_item(id);
	delete removedItem;
	NotifyDeductionDeleted(id);
}
