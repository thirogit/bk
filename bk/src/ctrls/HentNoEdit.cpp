#include "stdafx.h"
#include "HentNoEdit.h"
#include <data\types\countries\Countries.h>

BEGIN_MESSAGE_MAP(CHentNoEdit, CCountryEdit)
    
END_MESSAGE_MAP()

#define MASK_CHARACTER L'A'
#define DEFAULT_HENT_COUNTRY 616

CHentNoEdit::CHentNoEdit() : m_bIgnore(false)
{
}

std::wstring CHentNoEdit::EscapeMaskCharacter(const std::wstring& code2a)
{
	std::wstring escapedCode2a;

	for(int i = 0;i < code2a.length();i++)
	{
		if(code2a[i] == MASK_CHARACTER)
		{
			escapedCode2a += L'\\';
			escapedCode2a += MASK_CHARACTER;
		}
		else
		{
			escapedCode2a += code2a[i];
		}
	}
	return escapedCode2a;
}


bool CHentNoEdit::IsValidHentNo()
{
	return false;
}

void CHentNoEdit::SetHentNo(const HentNo& hentNo)
{
	if(!hentNo.IsNull())
	{
		SetCountry(hentNo.GetCountry());
		SetInputData(hentNo.GetNumber().c_str());
	}
	else
	{
		SetCountry(Countries::GetCountryByNumber(DEFAULT_HENT_COUNTRY));
		EmptyData(TRUE);
	}
}

HentNo CHentNoEdit::GetHentNo() const
{
	if(!IsInputEmpty())
		return HentNoFactory::Create(m_CurrentCountry,GetInputData());

	return HentNo();
}

void CHentNoEdit::SetCountry(const Country& country)
{	
	OnCountryChange(country);			
}

void CHentNoEdit::OnCountryChange(const Country& country)
{
	std::wstring sMaskPrefix = EscapeMaskCharacter(country.GetCountryCode2A());
	int numberPartLen = country.GetFarmNoLength()+country.GetHerdNoLength()-2;
	std::wstring sMask = sMaskPrefix + std::wstring(numberPartLen,L'#');
	SetMask(sMask.c_str());
	m_CurrentCountry = country;
	CCountryEdit::OnCountryChange(country);
}