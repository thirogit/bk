#pragma once

#include "../../SessionAwareWnd.h"
#include "../../context/ForwardHerdObserver.h"
#include "../../ctrls/xcolorstatic/XColorStatic.h"

class CHerdNumbersBar : public CSessionAwareWnd<CXColorStatic>, public ForwardHerdObserver
{
public:
	CHerdNumbersBar(uint32_t herdId);
	virtual ~CHerdNumbersBar();
	
	BOOL Create(const CRect & rect, CWnd * pParentWnd, UINT nID);
protected:
	void UpdateNumbers();

	virtual void  OnHerdCowAdded(const CowPresenceKey& key);
	virtual void  OnHerdCowRemoved(const CowPresenceKey& key);
	virtual void  OnHerdCowDeleted(const CowPresenceKey& key);
	virtual void  OnHerdCowMoved(const CowPresenceKey& fromKey, const CowPresenceKey& toKey);
	virtual void  OnHerdCowRevoked(const CowPresenceKey& fromKey, const CowPresenceKey& toKey);

private:
	uint32_t m_herdId;	
	
protected:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	
	DECLARE_MESSAGE_MAP()
};

