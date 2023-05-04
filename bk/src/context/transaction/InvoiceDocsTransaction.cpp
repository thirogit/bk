#include <stdafx.h>
#include "InvoiceDocsTransaction.h"
#include <boost/any.hpp>
#include <arrays\DeepDelete.h>
#include "TransactionException.h"
#include <string\TextUtils.h>
#include <data\runtime\InvoiceDocsScopeOperations.h>
#include <boost\foreach.hpp>

InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::InvoiceTransactionFacadeImpl(InvoiceDocsTransaction* pOwner)
{
	m_pOwner = pOwner;
}

InvoiceType InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetInvoiceType() const
{
	return  m_pOwner->GetInvoiceType();
}

const InvoiceDetails& InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetDetails() const
{
	return m_pOwner->GetDetails();
}

void InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::UpdateDetails(const InvoiceDetails& details)
{
	m_pOwner->UpdateDetails(details);
}

void InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::UpdateCow(ItemId id,const CowDetails& details)
{
	m_pOwner->UpdateCow(id,details);
}

void InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::UpdateEntry(ItemId id,const CowInvoiceEntryDetails& details)
{
	m_pOwner->UpdateEntry(id,details);
}

const InvoiceTransactionItem* InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetItemAt(int index)
{
	return m_pOwner->GetItemAt(index);
}

const InvoiceTransactionItem* InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetItem(ItemId id)
{
	return m_pOwner->GetItem(id);
}

count_t InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetItemCount() const
{
	return m_pOwner->GetItemCount();
}

void InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::AddObserver(InvoiceTransactionObserver* pObserver)
{
	m_pOwner->AddObserver(pObserver);
}

void InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::RemoveObserver(InvoiceTransactionObserver* pObserver)
{
	m_pOwner->RemoveObserver(pObserver);
}

count_t InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetDocCount()
{
	return m_pOwner->GetDocCount();
}

uint32_t InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetDocAt(int index)
{
	return m_pOwner->GetDocAt(index);
}

const InvoiceTransactionDeductionItem* InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetDeductionAt(int index)
{
	return m_pOwner->m_deductions.item_at(index);
}

const InvoiceTransactionDeductionItem* InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetDeduction(ItemId id)
{
	return m_pOwner->m_deductions.find_item(id);
}

count_t InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::GetDeductionCount() const
{
	return m_pOwner->m_deductions.size();
}

void InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::UpdateDeduction(ItemId id, const InvoiceDeduction& deduction)
{
	m_pOwner->UpdateDeduction(id, deduction);
}

ItemId InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::AddDeduction(const InvoiceDeduction& deduction)
{
	return m_pOwner->AddDeduction(deduction);
}

void InvoiceDocsTransaction::InvoiceTransactionFacadeImpl::RemoveDeduction(ItemId id)
{
	m_pOwner->RemoveDeduction(id);
}

//#################################################################################################

InvoiceDocsTransaction::InvoiceDocsTransaction(ITransactionInterface* pInterface) : m_State(TransactionState_Virgin),m_pInterface(pInterface),m_pFacade(NULL)
{
}

InvoiceDocsTransaction::~InvoiceDocsTransaction()
{
	if(m_State == TransactionState_Oppened)
	{
		Rollback();
	}	

	if(m_pFacade)
		delete m_pFacade;

	delete m_pInterface;
}


uint32_t InvoiceDocsTransaction::GetInvoiceNo() const
{
	return m_invoiceNo;
}

void InvoiceDocsTransaction::Open()
{
	if(m_State != TransactionState_Virgin)
	{
		throw TransactionException(L"invalid transaction state, expecting it to be in virgin state");
	}
	else
	{
		m_invoiceId = boost::any_cast<uint32_t>(m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetInvoiceId));
		m_invoiceNo = boost::any_cast<uint32_t>(m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetInvoiceNo));
		count_t itemCount = boost::any_cast<count_t>(m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetItemsCount));
		m_details.CopyFrom(boost::any_cast<InvoiceDetails>(m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetInvoiceDetails)));
		m_hent.CopyFrom(boost::any_cast<InvoiceHent>(m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetHent)));
		count_t deductionCount = boost::any_cast<count_t>(m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetDeductionsCount));
		m_invoiceType = boost::any_cast<InvoiceType>(m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetInvoiceType));


		for(int i = 0;i < itemCount;i++)
		{
			OperationArguments args;		
			args.add(i);
			boost::any itemTupleAny = m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetItemAt,args);
			InvoiceTransactionItemTuple itemTuple = boost::any_cast<InvoiceTransactionItemTuple>(itemTupleAny);

			InvoiceTransactionItem* item = CreateItemOutOf(itemTuple);

			m_items.push_back(item);			
			
		}

		for (int i = 0; i < deductionCount; i++)
		{
			OperationArguments args;
			args.add(i);
			boost::any deductionTupleAny = m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetDeductionAt, args);
			DeductionItemTuple deductionTuple = boost::any_cast<DeductionItemTuple>(deductionTupleAny);

			InvoiceTransactionDeductionItem* deductionItem = CreateDeductionItemOutOf(deductionTuple);
			m_deductions.add_item(deductionItem);

		}
		m_pFacade = new InvoiceTransactionFacadeImpl(this);
		m_State = TransactionState_Oppened;
	}
}

InvoiceTransactionItem* InvoiceDocsTransaction::CreateItemOutOf(const InvoiceTransactionItemTuple& itemTuple)
{
	InvoiceTransactionItem* item = new InvoiceTransactionItem(itemTuple.get<0>());
	item->SetCowId(itemTuple.get<1>());
	item->SetCowDetails(itemTuple.get<2>());
	item->SetEntry(itemTuple.get<3>());
	return item;
}

InvoiceTransactionDeductionItem* InvoiceDocsTransaction::CreateDeductionItemOutOf(const DeductionItemTuple& itemTuple)
{
	InvoiceTransactionDeductionItem* deductionItem = new InvoiceTransactionDeductionItem(itemTuple.get<0>());
	InvoiceDeduction deduction = itemTuple.get<1>();
	deductionItem->SetDetails(deduction.GetDetails());
	deductionItem->SetEnabled(deduction.GetIsEnabled());

	return deductionItem;
}

bool InvoiceDocsTransaction::IsUpdating() const
{
	ThrowIfNotOpen();
	return m_invoiceId != NULL_ID;
}
	
void InvoiceDocsTransaction::UpdateDetails(const InvoiceDetails& details)
{
	ThrowIfNotOpen();
	OperationArguments args;
	args.add(details);
	m_pInterface->Execute(InvoiceDocScopeOperations::Operation_SetInvoiceDetails,args);
	m_details.CopyFrom(details);
}

void InvoiceDocsTransaction::AddDoc(uint32_t docId)
{
	ThrowIfNotOpen();
	m_pInterface->Execute(InvoiceDocScopeOperations::Operation_AddDoc,OperationArguments::pack(docId));

	boost::any docItemCountAny = m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetDocItemCount,OperationArguments::pack(docId));
	count_t docItemCount = boost::any_cast<count_t>(docItemCountAny);
	
	for(int i = 0;i < docItemCount;i++)
	{
		boost::any docItemAny = m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetDocItemAt,OperationArguments::pack(docId,i));
		InvoiceTransactionItem* newItem = CreateItemOutOf(boost::any_cast<InvoiceTransactionItemTuple>(docItemAny));
		m_items.push_back(newItem);
		NotifyItemAdded(newItem->GetItemKey().GetItemId());
	}	
}

void InvoiceDocsTransaction::UpdateCow(ItemId id,const CowDetails& details)
{
	ThrowIfNotOpen();

	InvoiceTransactionItem* item = FindItem(id);
	m_pInterface->Execute(InvoiceDocScopeOperations::Operation_UpdateCow,OperationArguments::pack(item->GetItemKey(),details));
	item->SetCowDetails(details);
	NotifyItemUpdated(id);
}
void InvoiceDocsTransaction::UpdateEntry(ItemId id,const CowInvoiceEntryDetails& details)
{
	ThrowIfNotOpen();
	InvoiceTransactionItem* item = FindItem(id);
	m_pInterface->Execute(InvoiceDocScopeOperations::Operation_UpdateCowInvoiceEntry,OperationArguments::pack(id,details));
	NotifyItemUpdated(id);
}

void InvoiceDocsTransaction::DeleteDoc(uint32_t docId)
{
	ThrowIfNotOpen();	
	m_pInterface->Execute(InvoiceDocScopeOperations::Operation_DeleteDoc,OperationArguments::pack(docId));
	std::vector<InvoiceTransactionItem*> deltedItems(m_items.delete_items(docId));

	BOOST_FOREACH(InvoiceTransactionItem* item, deltedItems)
	{
		NotifyItemDeleted(item->GetItemKey().GetItemId());
		delete item;
	}
}

const InvoiceTransactionItem* InvoiceDocsTransaction::GetItemAt(int index)
{
	ThrowIfNotOpen();
	return m_items.get_at(index);
}

InvoiceTransactionItem* InvoiceDocsTransaction::FindItem(ItemId id)
{
	ThrowIfNotOpen();

	return m_items.find_item(id);
}


const InvoiceTransactionItem* InvoiceDocsTransaction::GetItem(ItemId id)
{
	return FindItem(id);
}

count_t InvoiceDocsTransaction::GetItemCount() const
{
	ThrowIfNotOpen();
	return m_items.size();
}

InvoiceType InvoiceDocsTransaction::GetInvoiceType() const
{
	return m_invoiceType;
}

void InvoiceDocsTransaction::SetInvoiceType(InvoiceType type)
{
	m_pInterface->Execute(InvoiceDocScopeOperations::Operation_SetInvoiceType, OperationArguments::pack(type));
	m_invoiceType = type;
}

void InvoiceDocsTransaction::Commit()
{
	ThrowIfNotOpen();

	m_pInterface->Commit();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
	DeepDelete(m_deductions);
}

void InvoiceDocsTransaction::Rollback()
{
	ThrowIfNotOpen();
	m_pInterface->Rollback();
	m_State = TransactionState_Closed;
	DeepDelete(m_items);
	DeepDelete(m_deductions);
}

const InvoiceDetails& InvoiceDocsTransaction::GetDetails() const
{
	return m_details;
}

void InvoiceDocsTransaction::ThrowIfNotOpen() const
{
	if(m_State != TransactionState_Oppened)
		throw TransactionException(L"transaction is not open");
}

DocsScopeInvoiceTransactionFacade* InvoiceDocsTransaction::GetFacade()
{
	return m_pFacade;
}

count_t InvoiceDocsTransaction::GetDocCount()
{
	boost::any countAny = m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetDocCount);
	return boost::any_cast<count_t>(countAny);
}

uint32_t InvoiceDocsTransaction::GetDocAt(int index)
{
	boost::any docKeyAny = m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetDocAt,OperationArguments::pack(index));
	return boost::any_cast<uint32_t>(docKeyAny);
}

const InvoiceHent& InvoiceDocsTransaction::GetHent() const
{
	ThrowIfNotOpen();
	return m_hent;
}

void InvoiceDocsTransaction::SetHent(const InvoiceHent& hent)
{
	ThrowIfNotOpen();
	m_pInterface->Execute(InvoiceDocScopeOperations::Operation_SetHent,OperationArguments::pack(hent));
	m_hent.CopyFrom(&hent);
}

InvoiceTransactionDeductionItem* InvoiceDocsTransaction::AcquireDeduction(ItemId id)
{
	InvoiceTransactionDeductionItem* deduction = m_deductions.find_item(id);
	if (!deduction)
		throw TransactionException(L"deduction_item_not_found");
	return deduction;
}

size_t InvoiceDocsTransaction::GetDeductionCount() const
{
	return m_deductions.size();
}

const InvoiceTransactionDeductionItem* InvoiceDocsTransaction::GetDeduction(ItemId id)
{
	return m_deductions.find_item(id);
}

const InvoiceTransactionDeductionItem* InvoiceDocsTransaction::GetDeductionAt(int index)
{
	return m_deductions.item_at(index);
}

void InvoiceDocsTransaction::UpdateDeduction(ItemId id, const InvoiceDeduction& deduction)
{
	InvoiceTransactionDeductionItem* deductionItem = AcquireDeduction(id);
	DeductionItemTuple deductionTuple(id, deduction);
	m_pInterface->Execute(InvoiceDocScopeOperations::Operation_UpdateDeduction, OperationArguments::pack(deductionTuple));

	deductionItem->SetDeduction(deduction);
	NotifyDeductionUpdated(id);
}

ItemId InvoiceDocsTransaction::AddDeduction(const InvoiceDeduction& deduction)
{
	boost::any addResultAny = m_pInterface->Execute(InvoiceDocScopeOperations::Operation_AddDeduction, OperationArguments::pack(deduction));
	ItemId itemId = boost::any_cast<ItemId>(addResultAny);

	boost::any getResultAny = m_pInterface->Execute(InvoiceDocScopeOperations::Operation_GetDeduction, OperationArguments::pack(itemId));
	DeductionItemTuple deductionTuple = boost::any_cast<DeductionItemTuple>(getResultAny);

	InvoiceTransactionDeductionItem* item = CreateDeductionItemOutOf(deductionTuple);
	m_deductions.add_item(item);
	NotifyDeductionAdded(itemId);

	return itemId;
}

void InvoiceDocsTransaction::RemoveDeduction(ItemId id)
{
	boost::any getResultAny = m_pInterface->Execute(InvoiceDocScopeOperations::Operation_RemoveDeduction, OperationArguments::pack(id));
	InvoiceTransactionDeductionItem* removedItem = m_deductions.remove_item(id);
	delete removedItem;
	NotifyDeductionDeleted(id);
}

