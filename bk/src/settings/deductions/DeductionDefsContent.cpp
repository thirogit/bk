#include "stdafx.h"
#include "DeductionDefsContent.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>
#include "DeductionDefinitionViewCreator.h"


DeductionDefsContent::DeductionDefsContent()
{
}

DeductionDefsContent::~DeductionDefsContent()
{
	DeepDelete(m_rows);
}

void DeductionDefsContent::QuietAdd(CRUDDeductionDefinition* definition)
{
	DeductionDefinitionView* view = m_viewCreator.CreateView(definition->GetId(), definition->Obj());
	m_rows.push_back(new DeductionDefinitionRow(view));
}

void DeductionDefsContent::Add(CRUDDeductionDefinition* definition)
{
	QuietAdd(definition);
	NotifyCountChanged();
}

void DeductionDefsContent::AddAll(CRUDDeductionDefinitionRange range)
{
	BOOST_FOREACH(CRUDDeductionDefinition* definition, range)
	{
		QuietAdd(definition);
	}
	NotifyCountChanged();
}

void DeductionDefsContent::DeleteAt(int row)
{
	auto it = m_rows.begin();
	std::advance(it, row);
	delete *it;
	m_rows.erase(it);
	NotifyCountChanged();
}

void DeductionDefsContent::UpdateAt(int row, CRUDDeductionDefinition* definition)
{
	m_viewCreator.RefreshView(definition->Obj(), m_rows[row]->Content());
	NotifyContentUpdated();
}

int DeductionDefsContent::GetRowCount() const
{
	return m_rows.size();
}

ListRow* DeductionDefsContent::GetRow(int row)
{
	return m_rows[row];
}

void DeductionDefsContent::SortContent(const SortAttributes& attributes)
{
}

void DeductionDefsContent::DeleteAll()
{
	DeepDelete(m_rows);
	m_rows.clear();
	NotifyCountChanged();
}

