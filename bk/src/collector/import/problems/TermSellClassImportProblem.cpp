#include "stdafx.h"
#include "TermSellClassImportProblem.h"

TermSellClassImportProblem::TermSellClassImportProblem(ItemId id, const std::wstring& classCd) : ImportProblem(id), m_classCd(classCd)
{
}

const std::wstring& TermSellClassImportProblem::GetClassCd() const
{
	return m_classCd;
}

ImportProblem* TermSellClassImportProblem::Clone() const
{
	return new TermSellClassImportProblem(GetId(), GetClassCd());
}

std::wstring TermSellClassImportProblem::DescribeProblem() const
{
	return L"no term sell class with code " + m_classCd;
}

