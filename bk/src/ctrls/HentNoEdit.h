#ifndef __HENTNOEDIT_H__
#define __HENTNOEDIT_H__

#include "countryedit/CountryEdit.h"
#include <data/types/HentNo.h>

class CHentNoEdit : public CCountryEdit
{
public:
	CHentNoEdit();
	bool IsValidHentNo();
	bool IsEmpty();

	void SetHentNo(const HentNo& hentNo);
	HentNo GetHentNo() const;
	void SetCountry(const Country& country);

protected:
	bool m_bIgnore;
	Country m_CurrentCountry;

	std::wstring EscapeMaskCharacter(const std::wstring& code2a);
	void OnCountryChange(const Country& country);

	//bool m_bDoValidate;
	//HentNo m_SetValue;
		
	DECLARE_MESSAGE_MAP()
};

#endif