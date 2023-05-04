#pragma once
#include "CowsTab.h"
#include "../../content/AllCowsListContent.h"
#include "../../print/DocumentArchetype.h"

class AllCowsTab : public CowsTab
{
public:
	AllCowsTab();
	BOOL Create(uint32_t herdId,UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);
private:
	uint32_t m_herdId;
	AllCowsListContent m_AllCows;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();	
	
	DECLARE_MESSAGE_MAP()
};

