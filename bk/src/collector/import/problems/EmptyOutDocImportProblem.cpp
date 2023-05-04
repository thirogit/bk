#include "stdafx.h"
#include "EmptyOutDocImportProblem.h"


EmptyOutDocImportProblem::EmptyOutDocImportProblem(ItemId id) : ImportProblem(id)
{
}

ImportProblem* EmptyOutDocImportProblem::Clone() const
{
	return new EmptyOutDocImportProblem(GetId());
}

std::wstring EmptyOutDocImportProblem::DescribeProblem() const
{
	return L"empty outdoc";
}


