#ifndef __CONTENTLISTCTRL_H__
#define __CONTENTLISTCTRL_H__

#include "ContentObserver.h"
#include "ListContent.h"
#include "../layoutlistctrl\LayoutListCtrl.h"

class CContentListCtrl : public CLayoutListCtrl, protected ContentObserver
{
public:
	CContentListCtrl();	
	virtual ~CContentListCtrl();
	
	void SetContent(ListContent* pContent);
	ListContent* GetContent();

	void UpdateItemCount();
	
protected:
	virtual void OnBeforeUpdate();
	virtual void OnCountChanged();
	virtual void OnContentChanged();
	virtual void OnAfterUpdate();

	virtual void PostCreate();
	virtual void SortBy(const SortAttributes& attributes);
	
	std::wstring GetCellText(int row,uint32_t colId);
	Color GetCellColor(int row,uint32_t colId);
	int GetCellImage(int row, uint32_t colId);
	

protected:
	ListContent* m_pContent;

	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()	
};

#endif