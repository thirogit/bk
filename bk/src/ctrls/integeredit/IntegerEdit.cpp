#include "stdafx.h"
#include "IntegerEdit.h"
#include <string/IntString.h>

CIntegerEdit::CIntegerEdit() : CNumericEdit(0)
{	
}

NullInt CIntegerEdit::GetInteger() const
{
	std::wstring sInteger = GetText();

	if(!sInteger.empty())
		return NullInt::FromString(sInteger);

	return NullInt();
}

void  CIntegerEdit::SetInteger(const NullInt& integer)
{
	if(!integer.IsNull())
	{
		SetText(integer.ToString());
	}
	else
	{
		Clear();
	}
}

void CIntegerEdit::SetInteger(int integer)
{
	SetText(IntString(integer));
}