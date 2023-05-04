#pragma once
#include "../ui/Tooltip.h"


class SettingsPropPage : public CPropertyPage
{
	DECLARE_DYNAMIC(SettingsPropPage)
public:
	
	SettingsPropPage();
	explicit SettingsPropPage(UINT nIDTemplate, UINT nIDCaption = 0, DWORD dwSize = sizeof(PROPSHEETPAGE));
	explicit SettingsPropPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0, DWORD dwSize = sizeof(PROPSHEETPAGE));
	
	SettingsPropPage(UINT nIDTemplate, UINT nIDCaption,
		UINT nIDHeaderTitle, UINT nIDHeaderSubTitle = 0, DWORD dwSize = sizeof(PROPSHEETPAGE));
	SettingsPropPage(LPCTSTR lpszTemplateName, UINT nIDCaption,
		UINT nIDHeaderTitle, UINT nIDHeaderSubTitle = 0, DWORD dwSize = sizeof(PROPSHEETPAGE));

	virtual ~SettingsPropPage();
		

protected:
	virtual BOOL OnInitDialog()
	{
		CPropertyPage::OnInitDialog();
		m_tooltips.Create(this);
		return FALSE;
	}
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		m_tooltips.RelayEvent(pMsg, this);
		return CPropertyPage::PreTranslateMessage(pMsg);
	}

protected:

	CToolTips		m_tooltips;

	/**
	* Adjusts the size of a checkbox or radio button control.
	* Since we always make the size of those bigger than 'necessary'
	* for making sure that translated strings can fit in those too,
	* this method can reduce the size of those controls again to only
	* fit the text.
	*/
	void AdjustControlSize(UINT nID);
	
};
