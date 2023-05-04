#include "stdafx.h"
#include "ImportProblemIdentifier.h"
#include <arrays/DeepDelete.h>
#include "SnapshotImportItem.h"
#include "HerdImportProblem.h"

ImportProblemIdentifier::ImportProblemIdentifier(SeasonSession* session) : m_session(session)
{
}

ImportProblemIdentifier::~ImportProblemIdentifier()
{
	
}

void ImportProblemIdentifier::Visit(InDocImportItem* item)
{
	DeepDelete(item->m_problems);
	item->m_problems.clear();

	const DownloadDoc* outdoc = item->GetInDoc();
	
	HentClientObject* hentCO = m_session->GetHent(outdoc->HentId);

	if (!hentCO)
	{
		item->m_problems.push_back(new MissingHentImportProblem(m_IdGenerator(), outdoc->Hent));
	}

	
}

void ImportProblemIdentifier::Visit(CowImportItem* item)
{
	DeepDelete(item->m_problems);
	item->m_problems.clear();

	const DownloadCow* cow = item->GetCow();

	std::wstring mainClassCd = cow->CowClass;
	if (!m_session->GetClassByCd(mainClassCd))
	{
		item->m_problems.push_back(new MainClassImportProblem(m_IdGenerator(), mainClassCd));
	}

	std::wstring termBuyClassCd = cow->TermBuyClass;
	if (!m_session->GetClassByCd(termBuyClassCd))
	{
		item->m_problems.push_back(new TermBuyClassImportProblem(m_IdGenerator(), termBuyClassCd));
	}

	std::wstring termSellClassCd = cow->TermSellClass;
	if (!m_session->GetClassByCd(termSellClassCd))
	{
		item->m_problems.push_back(new TermSellClassImportProblem(m_IdGenerator(), termSellClassCd));
	}

}

void ImportProblemIdentifier::Visit(OutDocImportItem* item)
{
	DeepDelete(item->m_problems);
	item->m_problems.clear();

	const DownloadDoc* outdoc = item->GetOutDoc();

	outdoc->HentId;
	
}

void ImportProblemIdentifier::Visit(SnapshotImportItem* item)
{
	DeepDelete(item->m_problems);
	item->m_problems.clear();

	const DownloadSnapshotHeader* snapshot = item->GetSnapshot();

	int herdIndex = snapshot->GetHerdIndex();
	IHerd* herd = m_session->GetHerdByIndex(herdIndex);
	if (!herd)
	{
		item->m_problems.push_back(new HerdImportProblem(m_IdGenerator(), herdIndex));
	}
}

void ImportProblemIdentifier::IdentifyProblems(ImportItem* item)
{
	

	
}




