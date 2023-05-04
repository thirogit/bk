#include "stdafx.h"
#include "MainClassImportProblem.h"
//#include <errortoken/ErrorToken.h>
//#include "HerdIntegritySolution.h"
//#include "HerdProblemDetectingVisitor.h"
//#include <string/TextUtils.h>

MainClassImportProblem::MainClassImportProblem(ItemId id, const std::wstring& classCd) :
	ImportProblem(id), m_classCd(classCd)
{
}


std::wstring MainClassImportProblem::DescribeProblem() const
{
	return L"no class " + m_classCd;
}

void MainClassImportProblem::Accept(ProblemVisitor* visitor)
{
	visitor->Visit(this);
}

//Solution* MainClassImportProblem::Solve(SolutionParameters& parameters)
//{
//	return new HerdIntegritySolution(parameters.type(), GetPath(), m_herdIndex, parameters.parameter<int>());
//}
//
//bool MainClassImportProblem::IsSameAs(IntegrityProblem* problem)
//{
//	HerdProblemDetectingVisitor visitor(m_herdIndex);
//	problem->Accept(&visitor);
//	return visitor.WasDetected();
//}

const std::wstring& MainClassImportProblem::GetClassCd() const
{
	return m_classCd;
}

ImportProblem* MainClassImportProblem::Clone() const
{
	return new MainClassImportProblem(GetId(), GetClassCd());
}
