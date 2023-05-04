#include "stdafx.h"
#include "AccountNoEdit.h"
#include <data/types/countries/Countries.h>
#include "../resource.h"
#include "../utils/StringLoader.h"

CAccountNoEdit::CAccountNoEdit() 
{	
}

bool CAccountNoEdit::IsValidAccountNo()
{
	int i,mod;
	wchar_t szIBAN[31],szIBANpart[9];
	std::wstring data;
	TCHAR iChar;

	if(IsInputEmpty()) return true;

	data = GetInputData();
	memset(szIBAN,0,sizeof(szIBAN));
	
	for(i = 0;i < 26;i++)
	{
		 iChar = data[i];
		 if(iChar > _T('9') || iChar < _T('0'))
		 		return false;
	}

	const wchar_t *szDataPtr = data.c_str();
	_stprintf_s(szIBAN, _T("%.24s%d%d%.2s"), &szDataPtr[2], 'P' - 55, 'L' - 55, szDataPtr);
	
	for(i=0,mod=0;i<30;i+=6)
	{
		_stprintf_s(szIBANpart, _T("%d%.6s"), mod, &szIBAN[i]);
		mod = _ttoi(szIBANpart)%97;
    }

	return (mod == 1);
}


BOOL CAccountNoEdit::OnValidate()
{
	if(!COXMaskedEdit::OnValidate()) return FALSE;

	if(!IsValidAccountNo())
	{		
		SetToolTipText(StringLoader(IDS_ERRASKFORVALIDACCOUNTNO).c_str());
		SetToolTipTitle(TTI_ERROR,StringLoader(IDS_ERRINVALIDACCOUNTNO).c_str());
		ShowToolTip();
		return FALSE;
	}
	return TRUE;
}

void CAccountNoEdit::PreSubclassWindow()
{
	CTipEditWrapper<COXMaskedEdit>::PreSubclassWindow();
	SetAccountCountry(Countries::GetCountryByNumber(616));
}


void CAccountNoEdit::ResetMask()
{
	const TCHAR* szMaskTemplate = _T("\\%c\\%c## #### #### #### #### #### ####");
	TCHAR szMaskBuffer[50];
	std::wstring code2a = m_country.GetCountryCode2A();
	_stprintf_s(szMaskBuffer,szMaskTemplate,code2a[0],code2a[1]);
	SetMask(szMaskBuffer);
}

void CAccountNoEdit::SetAccountCountry(const Country& country)
{
	m_country = country;
	ResetMask();
}

AccountNo CAccountNoEdit::GetAccountNo()
{
	if(IsFullPrompt())
	{
		return AccountNoFactory::Create(m_country,GetInputData());
	}
	return AccountNo();
}

void CAccountNoEdit::SetAccountNo(const AccountNo& accountNo)
{
	if(accountNo.IsNull())
	{
		EmptyData(TRUE);
	}
	else
	{
		SetAccountCountry(accountNo.GetCountry());
		SetInputData(accountNo.GetNumber().c_str(),0,FALSE);
	}
}