#include "stdafx.h"
#include "SnapshotImportSession.h"
#include "arrays\DeepDelete.h"
#include "items\ImportItemVisitor.h"
#include <boost\foreach.hpp>
#include "../data/DownloadDoc.h"
#include "problems/HentImportProblem.h"
#include "problems\EmptyInDocImportProblem.h"
#include "problems\EmptyOutDocImportProblem.h"
#include "problems\MainClassImportProblem.h"
#include "problems\TermBuyClassImportProblem.h"
#include "problems\TermSellClassImportProblem.h"

SnapshotImportSession::SessionImportItemVisitor::SessionImportItemVisitor(SnapshotImportSession* parent, SeasonSession* session) : m_parent(parent), m_session(session)
{
}


void SnapshotImportSession::SessionImportItemVisitor::Visit(InDocImportItemVisitorFacade* item)
{
	DownloadDoc* doc = item->GetInDoc();

	boost::optional<DownloadHent> hent = doc->Hent;
	if (hent)
	{
		HentClientObject* hentByIdCO = m_session->GetHent(hent->Id);
		if (!hentByIdCO)
		{
			HentClientObject* hentByNoCO = NULL;
			
			try
			{
				HentNo hentNo = HentNoFactory::FromString(hent->HentNo);
				hentByNoCO = m_session->GetHent(hentNo);
			}
			catch(...)	{ }

			item->AddProblem(new HentImportProblem(m_parent->m_IdGenerator(),hent.get(), (hentByNoCO != NULL) ? 1 : 0));
		}
	}

	CowImportItemRange cows = item->Cows();

	if (cows.empty())
	{
		item->AddProblem(new EmptyInDocImportProblem(m_parent->m_IdGenerator()));
	}

	BOOST_FOREACH(CowImportItem* cowItem, cows)
	{
		cowItem->Accept(this);
	}

}

void SnapshotImportSession::SessionImportItemVisitor::Visit(CowImportItemVisitorFacade* item)
{
	DownloadCow* cow = item->GetCow();

	
	if (!m_session->GetClassByCd(cow->CowClass))
	{
		item->AddProblem(new MainClassImportProblem(m_parent->m_IdGenerator(), cow->CowClass));
	}

	if (!m_session->GetClassByCd(cow->TermBuyClass))
	{
		item->AddProblem(new TermBuyClassImportProblem(m_parent->m_IdGenerator(), cow->TermBuyClass));
	}

	if (!m_session->GetClassByCd(cow->TermSellClass))
	{
		item->AddProblem(new TermSellClassImportProblem(m_parent->m_IdGenerator(), cow->TermSellClass));
	}	


	try
	{
		CowNo cowNo = CowNoFactory::FromString(cow->CowNo);
	}
	catch (CowNoException& e)
	{

	}
	
	
}

void SnapshotImportSession::SessionImportItemVisitor::Visit(OutDocImportItemVisitorFacade* item)
{
	DownloadDoc* doc = item->GetOutDoc();

	boost::optional<DownloadHent> hent = doc->Hent;
	if (hent)
	{
		HentClientObject* hentCO = m_session->GetHent(hent->Id);
		if (!hentCO)
		{
			try
			{
				HentNo hentNo = HentNoFactory::FromString(hent->HentNo);
				hentCO = m_session->GetHent(hentNo);
			}
			catch (...) {}

			if (!hentCO)
			{
				item->AddProblem(new HentImportProblem(m_parent->m_IdGenerator(), hent.get(), (hentCO != NULL) ? 1 : 0));
			}
		}
	}

	CowImportItemRange cows = item->Cows();

	if (cows.empty())
	{
		item->AddProblem(new EmptyOutDocImportProblem(m_parent->m_IdGenerator()));
	}

	BOOST_FOREACH(CowImportItem* cowItem, cows)
	{
		cowItem->Accept(this);
	}
}


SnapshotImportSession::SnapshotImportSession(const DownloadSnapshot* snapshot) : m_snapshotHeader(NULL_ID)
{
	m_snapshotHeader.CopyFrom(snapshot);

	std::multimap<uint32_t, DownloadCow*> cowsPerInDoc;
	std::vector<DownloadCow*> lonelyCows;
	std::multimap<uint32_t, DownloadCow*> cowsPerOutDoc;

	BOOST_FOREACH(DownloadCow* cow, snapshot->Cows())
	{
		if (cow->InDocId != NULL_ID)
		{
			cowsPerInDoc.insert(std::make_pair(cow->InDocId, cow));
		}
		else
		{
			lonelyCows.push_back(cow);
		}

		if (cow->OutDocId != NULL_ID)
		{
			cowsPerOutDoc.insert(std::make_pair(cow->OutDocId, cow));
		}
	}

	BOOST_FOREACH(DownloadDoc* doc, snapshot->InDocs())
	{
		InDocImportItem* indocItem = new InDocImportItem(m_IdGenerator());
		indocItem->GetInDoc() = *doc;
	}

	BOOST_FOREACH(DownloadDoc* doc, snapshot->OutDocs())
	{
		OutDocImportItem* indocItem = new OutDocImportItem(m_IdGenerator());
		indocItem->GetOutDoc() = *doc;
	}

}

SnapshotImportSession::SnapshotImportSession(SnapshotImportSession&& src) : m_snapshotHeader(NULL_ID)
{
}

SnapshotImportSession::SnapshotImportSession(const SnapshotImportSession& src) : m_snapshotHeader(NULL_ID)
{
	m_snapshotHeader = src.m_snapshotHeader;

	BOOST_FOREACH(CowImportItem* cowItem, src.m_lonelycows)
	{
		m_lonelycows.push_back(new CowImportItem(*cowItem));
	}

	BOOST_FOREACH(InDocImportItem* inDocItem, src.m_indocs)
	{
		m_indocs.push_back(new InDocImportItem(*inDocItem));
	}

	BOOST_FOREACH(OutDocImportItem* outDocItem, src.m_outdocs)
	{
		m_outdocs.push_back(new OutDocImportItem(*outDocItem));
	}		

	BOOST_FOREACH(ImportProblem* problem, src.m_problems)
	{
		m_problems.push_back(problem->Clone());
	}
}

SnapshotImportSession::~SnapshotImportSession()
{
	//delete m_snapshot;
	DeepDelete(m_problems);
	DeepDelete(m_lonelycows);
	DeepDelete(m_indocs);
	DeepDelete(m_outdocs);
}


//DownloadSnapshotHeader SnapshotImportSession::GetSnapshot()
//{
//	return m_snapshot;
//}

void SnapshotImportSession::IdentifyProblems(SeasonSession* session)
{

}


	



