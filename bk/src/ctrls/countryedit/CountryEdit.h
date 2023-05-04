#ifndef __COUNTRYEDIT_H__
#define __COUNTRYEDIT_H__

#include <data\types\countries\Country.h>
#include "../editwithbtnctrl/editwithbtnctrl.h"
#include "../countryflags/CountryFlagsImageList.h"

class CCountryEdit : public CEditWithBtnCtrl
{
public:
	CCountryEdit();
	void SetCountry(const Country& country);
protected:
	virtual void OnCountryChange(const Country& country);
	const Country GetCountry(unsigned int countryISONumber);
	const Country GetCountry(const std::wstring& countryCode2a);
	void CreateCountryMenu();

	virtual void OnBtnClick();
	
	unsigned int m_checkedCountry;
	CMenu m_CountryMenu;
	CountryFlagsImageList m_CountryFlags;

	virtual void PreSubclassWindow();

	afx_msg void OnCountry(UINT id);
	
	DECLARE_MESSAGE_MAP()
};


#endif