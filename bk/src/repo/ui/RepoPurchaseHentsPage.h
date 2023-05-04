#ifndef __REPOPURCHASEHENTSPAGE_H__
#define __REPOPURCHASEHENTSPAGE_H__

#include "../../ctrls/contentlistctrl\ContentListCtrl.h"
#include "../../ui/LayoutManager/ETSLayout.h"
#include "../../resource.h"
#include <data\repo\IPurchase.h>
#include "content/PurchaseHentListContent.h"

class CRepoPurchaseHentsPage : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CRepoPurchaseHentsPage)

public:
	CRepoPurchaseHentsPage();   
	virtual ~CRepoPurchaseHentsPage();	
	enum { IDD = IDD_REPO_ITEMS_VIEW };
	void UpdateFrom(IPurchase* pPurchase);
private:
	CContentListCtrl m_HentList;
	PurchaseHentListContent m_HentListContent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
		
	DECLARE_MESSAGE_MAP()

};

#endif