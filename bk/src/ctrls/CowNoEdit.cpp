#include "stdafx.h"
#include "CowNoEdit.h"
#include <data\types\countries\Countries.h>

BEGIN_MESSAGE_MAP(CCowNoEdit, CCountryEdit)
    
END_MESSAGE_MAP()

#define MASK_CHARACTER L'A'
#define DEFAULT_HENT_COUNTRY 616

CCowNoEdit::CCowNoEdit() : m_bIgnore(false)
{
}

std::wstring CCowNoEdit::EscapeMaskCharacter(const std::wstring& code2a)
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


bool CCowNoEdit::IsValidCowNo()
{
	return false;
}

void CCowNoEdit::SetCowNo(const CowNo& hentNo)
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

CowNo CCowNoEdit::GetCowNo()
{
	if(!IsInputEmpty() && IsFullPrompt())
		return CowNoFactory::Create(m_CurrentCountry,GetInputData());

	return CowNo();
}

void CCowNoEdit::SetCountry(const Country& country)
{	
	OnCountryChange(country);			
}

void CCowNoEdit::OnCountryChange(const Country& country)
{
	std::wstring sMaskPrefix = EscapeMaskCharacter(country.GetCountryCode2A());
	int numberPartLen = country.GetCowNoLength()-2;
	std::wstring sMask = sMaskPrefix + std::wstring(numberPartLen,L'#');
	SetMask(sMask.c_str());
	m_CurrentCountry = country;
	CCountryEdit::OnCountryChange(country);
}