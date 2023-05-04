#pragma once
#include "../ListTab.h"
#include "../../content\MoveDocsListContent.h"

class MoveDocsTab : public ListTab
{
public:
	MoveDocsTab();
	virtual ~MoveDocsTab();
	void SetContent(MoveDocsListContent *content);

protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	DECLARE_MESSAGE_MAP()
};