#pragma once

#include "ICowTrail.h"
#include <data/IDoc.h>
#include <data/IMoveDoc.h>
#include <list>
#include <data/ICow.h>

class CowTrail : public ICowTrail
{

	class CowFootprint : public ICowFootprint
	{
	public:
		CowFootprint(IDoc* pDoc,ICowEntry* pEntry);
		virtual IDoc* GetDoc() const;
		virtual ICowEntry* GetEntry() const;
	private:
		IDoc* m_pDoc;
		ICowEntry* m_pEntry;
	};

	class CowMovement : public ICowMovement
	{
	public:
		CowMovement(IMoveDoc* pMove,ICowEntry* pEntry);
		virtual IMoveDoc* GetDoc() const;
		virtual ICowEntry* GetEntry() const;
	private:
		IMoveDoc* m_pDoc;
		ICowEntry* m_pEntry;
	};

public:
	CowTrail(ICow* pCow);
	~CowTrail();
	virtual const ICowFootprint* GetEntry() const;
	virtual const ICowFootprint* GetExit() const;	
	virtual PtrEnumerator<ICowMovement> GetTrail();

	virtual bool IsStationary() const;
	
	void SetEntryFootprint(IDoc* pViaDoc,ICowEntry* pEntry);
	void SetExitFootprint(IDoc* pViaDoc,ICowEntry* pEntry);	
	void EraseExitFootprint();
	void EraseLastMovement();

	void AddFootprint(IMoveDoc* pFootprint,ICowEntry* pEntry);
	CowPath GetPath();
private:
	ICow* m_pCow;
	CowFootprint* m_pEntryFootprint;
	CowFootprint* m_pExitFootprint;
	std::list<CowMovement*> m_trail;
		
	

};
