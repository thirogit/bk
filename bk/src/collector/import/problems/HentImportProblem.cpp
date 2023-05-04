#include "stdafx.h"
#include "HentImportProblem.h"


HentImportProblem::HentImportProblem(ItemId id, const DownloadHent& hent, uint16_t solutions) : ImportProblem(id), m_problemHent(hent), m_solutions(solutions)
{
}
	
DownloadHent HentImportProblem::GetHent() const
{
	return m_problemHent;
}

ImportProblem* HentImportProblem::Clone() const
{	
	return new HentImportProblem(GetId(), m_problemHent, m_solutions);
}

std::wstring HentImportProblem::DescribeProblem() const
{
	return L"missing hent " + m_problemHent.Name + L" with hentno " + m_problemHent.HentNo;
}

