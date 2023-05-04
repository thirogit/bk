#include "stdafx.h"
#include "AbbyyString.h"



AbbyyString::AbbyyString( const CBstr& other )
{
	operator=(other);
}

AbbyyString& AbbyyString::operator=(const CBstr& right)
{
	m_sPlainString = ToPlainString(right);
	return *this;
}

const CBstr& AbbyyString::GetString() const
{
	return m_sPlainString;
}

AbbyyString::operator CBstr() const
{
	return m_sPlainString;
}

AbbyyString::operator BSTR() const
{
	return m_sPlainString;	
}


CBstr AbbyyString::ToPlainString(const CBstr& str)
{
	int len = str.Length();
	wchar_t *translatedStr = new wchar_t[len + 1];
	
	int i,j;
	wchar_t c;
	for(i = 0,j = 0;i < len;i++)
	{
		c = TranslateAbbyyCharacter(str[i]);
		if(c != 0)
		{
			translatedStr[j] = c; 
			j++;
		}
	}
	translatedStr[j] = 0;
	CBstr result(translatedStr);
	delete translatedStr;
	return result;
	
}

wchar_t AbbyyString::TranslateAbbyyCharacter(wchar_t chSpecial)
{
	const struct ABBYY_SPEC_CHAR_TRANSTALTION
	{
		wchar_t chSpecialCharacter;
		wchar_t chPlainCharacter;
	} translationTable[] =
	{
		{0x2028, 0x000A}, //Line break symbol.
		{0x2029, 0x000A}, //Paragraph break symbol.
		{0xFFFC, 0x0020}, //Object replacement character. Denotes an embedded picture inside the text.
		{0x0009, 0x0009}, //Tabulation
		{0x005E, 0x005E}, //Circumflex accent.
		{0x00AD, 0x0000}, //Soft hyphen.
		{0x0000, 0x0000}
	};

	const ABBYY_SPEC_CHAR_TRANSTALTION* pTranslation = translationTable;
	while(pTranslation->chSpecialCharacter != 0)
	{
		if(pTranslation->chSpecialCharacter == chSpecial)
			return pTranslation->chPlainCharacter;

		pTranslation++;
	}
	return chSpecial;
}
