#include "stdafx.h"
#include "TransactionCowEditor.h"
#include "TransactionException.h"
#include <string\TextUtils.h>

TransactionCowEditor::TransactionCowEditor(PlusDocTransactionFacade* pTransaction) : m_pTransaction(pTransaction)
{
			
}

void TransactionCowEditor::Edit(ItemId id)
{
	const DocItem* item = m_pTransaction->GetItem(id);
	if(!item)
		throw TransactionException(TextUtils::FormatA(L"%s item not found in transaction",ItemIdText::ToString(id).c_str()));

	m_itemId = id;
	m_Details.CopyFrom(item->GetCowDetails());
}

bool TransactionCowEditor::IsEditing() const
{
	return true;
}

void TransactionCowEditor::Save()
{
	if(m_itemId.is_nil())
		throw TransactionException(L"editor was not initialized");

	m_pTransaction->UpdateCow(m_itemId,m_Details);
}
