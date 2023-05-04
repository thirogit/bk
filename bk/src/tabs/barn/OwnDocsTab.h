#pragma once
#include "../ListTab.h"
#include "../../content\OwnDocsListContent.h"

class OwnDocsTab : public ListTab
{
public:
	OwnDocsTab();
	virtual ~OwnDocsTab();
	void SetContent(OwnDocsListContent *content);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	DECLARE_MESSAGE_MAP()
};

