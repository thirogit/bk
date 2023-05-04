#include "stdafx.h"
#include "TransactionNewCowEditor.h"

TransactionNewCowEditor::TransactionNewCowEditor(PlusDocTransactionFacade* pTransactionFacade) : 
	m_pTransactionFacade(pTransactionFacade)
{
}

bool TransactionNewCowEditor::IsEditing() const
{
	return false;
}

void TransactionNewCowEditor::Save()
{
	m_pTransactionFacade->PlusCow(m_Details);
}