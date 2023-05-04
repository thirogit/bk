#include "stdafx.h"
#include "InvoiceDeductionsContent.h"
#include <arrays\DeepDelete.h>
#include "../calculation/InvoiceCalculation.h"
#include <boost/foreach.hpp>
#include "../view/DeductionElementViewCreator.h"

InvoiceDeductionsContent::InvoiceDeductionsContent() : m_pCalculation(NULL),m_pSession(NULL)
{

}

InvoiceDeductionsContent::~InvoiceDeductionsContent()
{
	Destroy();
}

void InvoiceDeductionsContent::Create(InvoiceCalculation* pCalculation,SeasonSession* pSession)
{
	m_pCalculation = pCalculation;
	m_pCalculation->AddObserver(this);
	m_pSession = pSession;
	
	DeductionElements deductions = m_pCalculation->Deductions();
	

	BOOST_FOREACH(const DeductionElement* deduction, deductions)
	{
		AddRow(deduction);
	}
	
}

void InvoiceDeductionsContent::AddRow(const DeductionElement* deduction)
{
	DeductionElementViewCreator pViewCreator(m_pSession);
	DeductionElementView* pView = pViewCreator.CreateView(deduction);
	DeductionElementRow* pRow = new DeductionElementRow(pView);
	m_rows.push_back(pRow);
}

DeductionElementRow* InvoiceDeductionsContent::FindRow(ItemId id)
{
	boost::multi_index::index<RowIndex,tag_itemId>::type& indexById = boost::multi_index::get<tag_itemId>(m_rows);
	auto rowIt = indexById.find(id);
	if(rowIt != indexById.end())
		return *rowIt;

	return NULL;
}

void InvoiceDeductionsContent::ElementChanged(const CalculationElement* pElement)
{
	RefreshAllRows();
	NotifyContentUpdated();
}

void InvoiceDeductionsContent::ElementDeleted(ItemId id)
{
	RefreshAllRows();
	NotifyContentUpdated();
}

void InvoiceDeductionsContent::ElementAdded(const CalculationElement* pElement)
{
	RefreshAllRows();
	NotifyContentUpdated();
}

void InvoiceDeductionsContent::DeductionChanged(const DeductionElement* pDeduction)
{
	DeductionElementRow* row = FindRow(pDeduction->GetId());
	if (row)
	{
		DeductionElementViewCreator viewCreator(m_pSession);
		viewCreator.RefreshView(pDeduction, row->Content());
		NotifyContentUpdated();
	}	
}

void InvoiceDeductionsContent::DeductionDeleted(ItemId id)
{
	boost::multi_index::index<RowIndex, tag_itemId>::type& indexById = boost::multi_index::get<tag_itemId>(m_rows);
	auto rowIt = indexById.find(id);
	if (rowIt != indexById.end())
	{
		indexById.erase(rowIt);
	}	
	NotifyCountChanged();
}

void InvoiceDeductionsContent::DeductionAdded(const DeductionElement* pDeduction)
{
	AddRow(pDeduction);
	NotifyCountChanged();
}

void InvoiceDeductionsContent::Destroy()
{
	if (m_pCalculation)
	{
		m_pCalculation->RemoveObserver(this);
		m_pCalculation = NULL;
	}

	DeepDelete(m_rows);
	m_rows.clear();
	m_pSession = NULL;
}

int InvoiceDeductionsContent::GetRowCount() const
{
	return m_rows.size();
}

ListRow* InvoiceDeductionsContent::GetRow(int row)
{
	return GetDeductionRowAt(row);
}

DeductionElementRow* InvoiceDeductionsContent::GetDeductionRowAt(int row)
{
	return *(m_rows.begin()+row);
}


void InvoiceDeductionsContent::SortContent(const SortAttributes& attributes)
{
}

void InvoiceDeductionsContent::RefreshAllRows()
{
	DeductionElements deductions = m_pCalculation->Deductions();
	DeductionElementViewCreator pViewCreator(m_pSession);

	BOOST_FOREACH(const DeductionElement* deduction, deductions)
	{
		ItemId deductionId = deduction->GetId();
		DeductionElementRow* row = FindRow(deductionId);
		if (row != NULL)
		{
			pViewCreator.RefreshView(deduction, row->Content());
		}
		
	}
}