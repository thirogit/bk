#include "stdafx.h"
#include "CowTrail.h"
#include "../../arrays/DeepDelete.h"
#include "../runtime/LogicException.h"
#include "../../string/TextUtils.h"
#include "../../data/utils/GetDocHerd.h"

CowTrail::CowFootprint::CowFootprint(IDoc* pDoc,ICowEntry* pEntry) : m_pDoc(pDoc), m_pEntry(pEntry)
{
}

IDoc* CowTrail::CowFootprint::GetDoc() const
{
	return m_pDoc;
}

ICowEntry* CowTrail::CowFootprint::GetEntry() const
{
	return m_pEntry;
}

//-------------------------------------------------------------------------------------------------

CowTrail::CowMovement::CowMovement(IMoveDoc* pMove,ICowEntry* pEntry) : m_pDoc(pMove), m_pEntry(pEntry)
{
}

IMoveDoc* CowTrail::CowMovement::GetDoc() const
{
	return m_pDoc;
}

ICowEntry* CowTrail::CowMovement::GetEntry() const
{
	return m_pEntry;
}

//-------------------------------------------------------------------------------------------------
CowTrail::CowTrail(ICow* pCow) : m_pCow(pCow),m_pEntryFootprint(NULL),m_pExitFootprint(NULL)
{
}

CowTrail::~CowTrail()
{
	if(m_pEntryFootprint)
		delete m_pEntryFootprint;
	if(m_pExitFootprint)
		delete m_pExitFootprint;

	DeepDelete(m_trail);
}

const ICowFootprint* CowTrail::GetEntry() const
{
	return m_pEntryFootprint;
}

const ICowFootprint* CowTrail::GetExit() const
{
	return m_pExitFootprint;
}

PtrEnumerator<ICowMovement> CowTrail::GetTrail()
{
	return PtrEnumerator<ICowMovement>(m_trail.begin(),m_trail.end());
}

bool CowTrail::IsStationary() const
{
	return (m_pExitFootprint == NULL && m_trail.size() == 0);
}

void CowTrail::SetEntryFootprint(IDoc* pViaDoc,ICowEntry* pEntry)
{
	if (m_pEntryFootprint != NULL)
		BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"cow %d already has an entry footprint via doc %d, cannot set it via doc %d", m_pCow->GetId(), m_pEntryFootprint->GetDoc()->GetId(), pViaDoc->GetId())));
	m_pEntryFootprint = new CowFootprint(pViaDoc,pEntry);
}

void CowTrail::SetExitFootprint(IDoc* pViaDoc,ICowEntry* pEntry)
{
	if(m_pExitFootprint != NULL)
		BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"cow %d already has a exit footprint via doc %d, cannot set it via doc %d", m_pCow->GetId(), m_pExitFootprint->GetDoc()->GetId(), pViaDoc->GetId())));
	m_pExitFootprint = new CowFootprint(pViaDoc,pEntry);
}

void CowTrail::AddFootprint(IMoveDoc* pFootprint,ICowEntry* pEntry)
{
	if(m_trail.size() > 0)
	{
		std::list<CowMovement*>::reference lastFootprint = m_trail.back();
		IMoveDoc* pLastMoveDoc = lastFootprint->GetDoc();

		if(pLastMoveDoc->GetDstHerdId() != pFootprint->GetSrcHerdId())
			BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"cow %d, adding move that wrongly originated from src herd %d, its should originate from herd %d",
								m_pCow->GetId(),pFootprint->GetSrcHerdId(),pLastMoveDoc->GetDstHerdId())));

		m_trail.push_back(new CowMovement(pFootprint,pEntry));

	}
	else
	{
		if(m_pEntryFootprint == NULL)
			BOOST_THROW_EXCEPTION(LogicException(TextUtils::FormatA(L"cow %d does not have entry footprint, cannot add move doc %d", m_pCow->GetId(), pFootprint->GetId())));
		
		m_trail.push_back(new CowMovement(pFootprint,pEntry));
	}	
}

CowPath CowTrail::GetPath()
{
	CowPath path;
	path.AddStep(GetDocHerd(m_pEntryFootprint->GetDoc()),m_pEntryFootprint->GetDoc(),m_pEntryFootprint->GetEntry());

	std::for_each(m_trail.begin(),m_trail.end(),[&path](CowMovement* pMovement)
												{
													path.AddStep(pMovement->GetDoc()->GetDstHerdId(),pMovement->GetDoc(),pMovement->GetEntry());
												});
	if(m_pExitFootprint)
	{
		path.AddStep(GetDocHerd(m_pExitFootprint->GetDoc()),m_pExitFootprint->GetDoc(),m_pExitFootprint->GetEntry());
	}
	
	return std::move(path);
}

void CowTrail::EraseExitFootprint()
{
	if(m_pExitFootprint)
	{
		delete m_pExitFootprint;
		m_pExitFootprint = NULL;
	}
}

void CowTrail::EraseLastMovement()
{
	if(!m_trail.empty())
	{
		CowMovement* pLastMove = m_trail.back();
		m_trail.pop_back();
		delete pLastMove;
	}
}