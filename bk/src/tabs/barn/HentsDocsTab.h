#pragma once
#include "../ListTab.h"
#include "../../content\HentsDocsListContent.h"

class HentsDocsTab : public ListTab
{
public:
	HentsDocsTab();
	virtual ~HentsDocsTab();
	//void SetContent(HentsDocsListContent &content);

protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	DECLARE_MESSAGE_MAP()
};