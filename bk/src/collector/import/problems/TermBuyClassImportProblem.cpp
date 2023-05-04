#include "stdafx.h"
#include "TermBuyClassImportProblem.h"




TermBuyClassImportProblem::TermBuyClassImportProblem(ItemId id, const std::wstring& classCd) : ImportProblem(id), m_classCd(classCd)
{
}

const std::wstring& TermBuyClassImportProblem::GetClassCd() const
{
	return m_classCd;
}

ImportProblem* TermBuyClassImportProblem::Clone() const
{
	return new TermBuyClassImportProblem(GetId(), GetClassCd());
}

std::wstring TermBuyClassImportProblem::DescribeProblem() const
{
	return L"no term buy class with code " + m_classCd;
}

