#pragma once

#include "IHerdFootprint.h"
#include <list>
#include <arrays/Enumerator.h>

class CowPath
{
	class HerdFootprint : public IHerdFootprint
	{
	public:
		HerdFootprint(const HerdFootprint& src);
		HerdFootprint(uint32_t herdId,IDoc* pDoc,ICowEntry*	pEntry);

		void SetPrevStep(IHerdFootprint* prevStep);
		void SetNextStep(IHerdFootprint* nextStep);

		virtual IHerdFootprint* PrevStep();
		virtual IHerdFootprint* NextStep();
		virtual uint32_t GetHerdId() const;
		virtual IDoc* GetStepDoc() const;
		virtual ICowEntry* GetStepEntry() const;
	private:
		IHerdFootprint* m_prev;
		IHerdFootprint* m_next;
		uint32_t	m_herdId;
		IDoc*		m_pDoc;
		ICowEntry*	m_pEntry;
	};


public:
	CowPath();
	CowPath(CowPath&& src);
	~CowPath();
	CowPath(const CowPath& src);
	CowPath& operator=(const CowPath& right);
	PtrEnumerator<IHerdFootprint> GetSteps();
	IHerdFootprint* GetFirstStep();
	IHerdFootprint* GetLastStep();

	IHerdFootprint* GetLastStepInHerd(uint32_t herdId);
	IHerdFootprint* GetFootprint(IDoc* pDoc);
	
	void AddStep(uint32_t herdId,IDoc* pDoc,ICowEntry* pEntry);
private:
	std::list<HerdFootprint*> m_steps;
};

