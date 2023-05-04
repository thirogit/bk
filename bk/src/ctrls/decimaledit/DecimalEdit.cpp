#include "stdafx.h"
#include "DecimalEdit.h"

CDecimalEdit::CDecimalEdit(int precision/* = 0*/) : CNumericEdit(precision), m_Precision(precision)
{
	
}

NullDecimal CDecimalEdit::GetDecimal() const
{
	std::wstring sDecimal = GetText();

	if(!sDecimal.empty())
		return NullDecimal::FromString(sDecimal);

	return NullDecimal();
}

void  CDecimalEdit::SetDecimal(const NullDecimal& decimal)
{
	if(decimal.IsNull())
	{
		SetText(decimal.ToString(m_Precision));
	}
	else
	{
		Clear();
	}
}

void CDecimalEdit::SetDecimal(const Decimal& decimal)
{
	SetText(decimal.ToString(m_Precision));
}