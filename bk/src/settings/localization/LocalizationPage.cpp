#include "stdafx.h"
#include "LocalizationPage.h"


IMPLEMENT_DYNAMIC(LocalizationPage, SettingsPropPage)

LocalizationPage::LocalizationPage() : SettingsPropPage(LocalizationPage::IDD)
{
}

LocalizationPage::~LocalizationPage()
{
}

void LocalizationPage::DoDataExchange(CDataExchange* pDX)
{
	SettingsPropPage::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(LocalizationPage, SettingsPropPage)
	/*ON_BN_CLICKED(IDC_BUGTRAQREMOVEBUTTON, OnRemove)
	ON_BN_CLICKED(IDC_BUGTRAQEDITBUTTON, OnEdit)
	ON_BN_CLICKED(IDC_BUGTRAQADDBUTTON, OnAdd)*/
	
END_MESSAGE_MAP()

BOOL LocalizationPage::OnInitDialog()
{
	SettingsPropPage::OnInitDialog();

	

	return TRUE;
}

BOOL LocalizationPage::OnApply()
{
	return SettingsPropPage::OnApply();
}
