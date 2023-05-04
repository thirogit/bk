#include "stdafx.h"
#include "DocumentSelection.h"
#include <arrays\DeepDelete.h>

DocumentSelection::DocumentSelection()
{
}

DocumentSelection::~DocumentSelection()
{
}

DocumentSelection::DocumentSelection(const DocumentSelection& src)
{
	auto it = src.m_archetypes.begin();
	auto end = src.m_archetypes.end();

	while(it != end)
		m_archetypes.push_back(new DocumentArchetype(**it++));
}

DocumentSelection::DocumentSelection(DocumentSelection&& src)
{
	std::copy(src.m_archetypes.begin(),src.m_archetypes.end(),std::back_inserter(m_archetypes));
	src.m_archetypes.clear();
}

PtrEnumerator<DocumentArchetype> DocumentSelection::Enum()
{
	return PtrEnumerator<DocumentArchetype>(m_archetypes);
}

bool DocumentSelection::IsEmpty() const
{
	return m_archetypes.size() == 0;

}