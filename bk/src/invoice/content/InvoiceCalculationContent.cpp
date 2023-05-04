#include "stdafx.h"
#include "InvoiceCalculationContent.h"
#include <arrays\DeepDelete.h>
#include "../calculation/InvoiceCalculation.h"
#include <boost/foreach.hpp>

InvoiceCalculationContent::InvoiceCalculationContent(CalculationElementViewFactory& factory) : m_pCalculation(NULL),m_pSession(NULL),m_factory(factory)
{

}

InvoiceCalculationContent::~InvoiceCalculationContent()
{
	Destroy();
}

void InvoiceCalculationContent::Create(InvoiceCalculation* pCalculation,SeasonSession* pSession)
{
	m_pCalculation = pCalculation;
	m_pCalculation->AddObserver(this);
	m_pSession = pSession;
	
	CalculationElements elements = m_pCalculation->Elements();
	CalculationViewCreator* pViewCreator = m_factory(pSession);		

	BOOST_FOREACH(const CalculationElement* pElement,elements)
	{
		CalculationElementView* pView = pViewCreator->CreateView(pElement);
		CalculationElementRow* pRow = new CalculationElementRow(pView);
		m_rows.push_back(pRow);
	}

	delete pViewCreator;
	

}

CalculationElementRow* InvoiceCalculationContent::FindRow(ItemId id)
{
	boost::multi_index::index<RowIndex,tag_itemId>::type& indexById = boost::multi_index::get<tag_itemId>(m_rows);
	auto rowIt = indexById.find(id);
	if(rowIt != indexById.end())
		return *rowIt;

	return NULL;
}

void InvoiceCalculationContent::ElementChanged(const CalculationElement* pElement)
{
	CalculationElementRow* pRow = FindRow(pElement->GetId());			
	CalculationViewCreator* pViewCreator = m_factory(m_pSession);		
	pViewCreator->RefreshView(pElement,pRow->Content());		
	delete pViewCreator;	
	NotifyContentUpdated();
}

void InvoiceCalculationContent::ElementDeleted(ItemId id)
{
	boost::multi_index::index<RowIndex,tag_itemId>::type& indexByItemId = boost::multi_index::get<tag_itemId>(m_rows);
	auto rowIt = indexByItemId.find(id);
	CalculationElementRow* pRow = *rowIt;
	indexByItemId.erase(rowIt);
	delete pRow;
	NotifyCountChanged();
}

void InvoiceCalculationContent::ElementAdded(const CalculationElement* pElement)
{
			
	CalculationViewCreator* pViewCreator = m_factory(m_pSession);		
	CalculationElementView* pView = pViewCreator->CreateView(pElement);
	CalculationElementRow* pRow = new CalculationElementRow(pView);
	m_rows.push_back(pRow);
	delete pViewCreator;
	
	NotifyCountChanged();
}

void InvoiceCalculationContent::Destroy()
{
	if(m_pCalculation)
		m_pCalculation->RemoveObserver(this);

	DeepDelete(m_rows);
	m_rows.clear();
	m_pSession = NULL;
}

int InvoiceCalculationContent::GetRowCount() const
{
	return m_rows.size();
}

ListRow* InvoiceCalculationContent::GetRow(int row)
{
	return GetCalculationRow(row);
}

CalculationElementRow* InvoiceCalculationContent::GetCalculationRow(int row)
{
	return *(m_rows.begin()+row);
}


void InvoiceCalculationContent::SortContent(const SortAttributes& attributes)
{
}