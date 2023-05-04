#include "stdafx.h"
#include "ImportProblem.h"

ImportProblem::ImportProblem(ItemId id) : m_id(id)
{
}

ItemId ImportProblem::GetId() const
{
	return m_id;
}



