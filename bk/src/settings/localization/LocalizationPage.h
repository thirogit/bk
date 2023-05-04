#pragma once
#include "../../resource.h"
#include "../SettingsPropPage.h"
#include "../../uitypes/Color.h"
#include <types/NullDecimal.h>

class LocalizationPage : public SettingsPropPage
{
	DECLARE_DYNAMIC(LocalizationPage)

public:
	LocalizationPage();
	virtual ~LocalizationPage();

	enum { IDD = IDD_LOCALIZATION };
private:
	BOOL m_bCountry1Enabled;
	BOOL m_bCountry1Enabled;
	BOOL m_bCountry1Enabled;
	
	uint32_t m_country1;
	uint32_t m_country2;
	uint32_t m_country3;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	virtual void DoDataExchange(CDataExchange* pDX);  	
	

	DECLARE_MESSAGE_MAP()
};
