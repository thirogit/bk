#include "stdafx.h"
#include "CowPath.h"
#include <arrays/DeepDelete.h>
#include "../../data/DocKey.h"

CowPath::HerdFootprint::HerdFootprint(const HerdFootprint& src)
{
	m_prev = src.m_prev;
	m_next = src.m_next;
	m_herdId = src.m_herdId;
	m_pDoc = src.m_pDoc;
	m_pEntry = src.m_pEntry;
}

CowPath::HerdFootprint::HerdFootprint(uint32_t herdId,IDoc* pDoc,ICowEntry*	pEntry) : m_prev(NULL),m_next(NULL)
{
	m_herdId = herdId;
	m_pDoc = pDoc;
	m_pEntry = pEntry;
}

void CowPath::HerdFootprint::SetPrevStep(IHerdFootprint* prevStep)
{
	m_prev = prevStep;
}

void CowPath::HerdFootprint::SetNextStep(IHerdFootprint* nextStep)
{
	m_next = nextStep;
}

IHerdFootprint* CowPath::HerdFootprint::PrevStep()
{
	return m_prev;
}

IHerdFootprint* CowPath::HerdFootprint::NextStep()
{
	return m_next;
}

uint32_t CowPath::HerdFootprint::GetHerdId() const
{
	return m_herdId;
}

IDoc* CowPath::HerdFootprint::GetStepDoc() const
{
	return m_pDoc;
}

ICowEntry* CowPath::HerdFootprint::GetStepEntry() const
{
	return m_pEntry;
}

//-------------------------------------------------------------------------------------------------

CowPath::CowPath()
{
}

CowPath::CowPath(CowPath&& src)
{
	m_steps = std::move(src.m_steps);	
}

CowPath::~CowPath()
{
	DeepDelete(m_steps);
}

CowPath::CowPath(const CowPath& src)
{
	operator=(src);
}

CowPath& CowPath::operator=(const CowPath& right)
{
	DeepDelete(m_steps);
	m_steps.clear();


	std::for_each(right.m_steps.begin(),
				  right.m_steps.end(),
				  [this](HerdFootprint* pFootprint)
				  {
					  AddStep(pFootprint->GetHerdId(),pFootprint->GetStepDoc(),pFootprint->GetStepEntry());
				  });

	return *this;

}

PtrEnumerator<IHerdFootprint> CowPath::GetSteps()
{
	return PtrEnumerator<IHerdFootprint>(m_steps);
}

IHerdFootprint* CowPath::GetFirstStep()
{
	if(!m_steps.empty())
		return m_steps.front();

	return NULL;
}

IHerdFootprint* CowPath::GetLastStep()
{
	if(!m_steps.empty())
		return m_steps.back();

	return NULL;
}

void CowPath::AddStep(uint32_t herdId,IDoc* pDoc,ICowEntry* pEntry)
{
	HerdFootprint* pLastStep = NULL;
	if(!m_steps.empty())
		pLastStep = m_steps.back();

	HerdFootprint* pStep = new HerdFootprint(herdId,pDoc,pEntry);
	m_steps.push_back(pStep);

	if(pLastStep)
		pLastStep->SetNextStep(pStep);

	pStep->SetPrevStep(pLastStep);
}


IHerdFootprint* CowPath::GetLastStepInHerd(uint32_t herdId)
{
	auto it = std::find_if(m_steps.rbegin(),m_steps.rend(),
				[herdId](HerdFootprint* pFootprint) -> bool
				{
					return (pFootprint->GetHerdId() == herdId);
				});

	if(it != m_steps.rend())
		return *it;

	return NULL;
}

IHerdFootprint* CowPath::GetFootprint(IDoc* pDoc)
{
	auto it = std::find_if(m_steps.begin(), m_steps.end(),
		[pDoc](HerdFootprint* pFootprint) -> bool
	{
		IDoc* stepDoc = pFootprint->GetStepDoc();
		return DocKey(stepDoc->GetDocType(), stepDoc->GetId()) == DocKey(pDoc->GetDocType(), pDoc->GetId());
	});

	if (it != m_steps.end())
		return *it;

	return NULL;
}
