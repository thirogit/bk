#ifndef __COWNOEDIT_H__
#define __COWNOEDIT_H__

#include "countryedit/CountryEdit.h"
#include <data/types/CowNo.h>

class CCowNoEdit : public CCountryEdit
{
public:
	CCowNoEdit();
	bool IsValidCowNo();
	bool IsEmpty();

	void SetCowNo(const CowNo& cowNo);
	CowNo GetCowNo();
	void SetCountry(const Country& country);

protected:
	bool m_bIgnore;
	Country m_CurrentCountry;

	std::wstring EscapeMaskCharacter(const std::wstring& code2a);
	void OnCountryChange(const Country& country);
	
	DECLARE_MESSAGE_MAP()
};

#endif