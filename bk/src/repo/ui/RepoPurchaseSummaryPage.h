#ifndef __REPOPURCHASESUMMARYPAGE_H__
#define __REPOPURCHASESUMMARYPAGE_H__

#include "../../ui/LayoutManager/ETSLayout.h"
#include "../../resource.h"
#include <data\repo\IPurchase.h>

class CRepoPurchaseSummaryPage : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CRepoPurchaseSummaryPage)

public:
	CRepoPurchaseSummaryPage();   
	virtual ~CRepoPurchaseSummaryPage();	
	enum { IDD = IDD_REPO_PURCHASE_SUMMARY };
	void UpdateFrom(IPurchase* pPurchase);
private:
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
		
	DECLARE_MESSAGE_MAP()

};

#endif