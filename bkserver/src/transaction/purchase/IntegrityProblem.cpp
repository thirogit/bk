#include "stdafx.h"
#include "IntegrityProblem.h"

IntegrityProblem::IntegrityProblem(ItemId id, const PurchaseObjPath& path) : m_id(id), m_path(path)
{
}

ItemId IntegrityProblem::GetId() const
{
	return m_id;
}

const PurchaseObjPath& IntegrityProblem::GetPath() const
{
	return m_path;
}

