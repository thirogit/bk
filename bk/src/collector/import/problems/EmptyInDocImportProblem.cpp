#include "stdafx.h"
#include "EmptyInDocImportProblem.h"

EmptyInDocImportProblem::EmptyInDocImportProblem(ItemId id) : ImportProblem(id)
{
}

ImportProblem* EmptyInDocImportProblem::Clone() const
{
	return new EmptyInDocImportProblem(GetId());
}

std::wstring EmptyInDocImportProblem::DescribeProblem() const
{
	return L"empty indoc";
}
