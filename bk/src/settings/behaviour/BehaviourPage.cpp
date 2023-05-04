#include "stdafx.h"
#include "BehaviourPage.h"


IMPLEMENT_DYNAMIC(BehaviourPage, SettingsPropPage)

BehaviourPage::BehaviourPage() : SettingsPropPage(BehaviourPage::IDD)
{
}

BehaviourPage::~BehaviourPage()
{
}

void BehaviourPage::DoDataExchange(CDataExchange* pDX)
{
	SettingsPropPage::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(BehaviourPage, SettingsPropPage)
	/*ON_BN_CLICKED(IDC_BUGTRAQREMOVEBUTTON, OnRemove)
	ON_BN_CLICKED(IDC_BUGTRAQEDITBUTTON, OnEdit)
	ON_BN_CLICKED(IDC_BUGTRAQADDBUTTON, OnAdd)*/
	
END_MESSAGE_MAP()

BOOL BehaviourPage::OnInitDialog()
{
	SettingsPropPage::OnInitDialog();

	

	return TRUE;
}

BOOL BehaviourPage::OnApply()
{
	return SettingsPropPage::OnApply();
}
