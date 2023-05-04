#include "stdafx.h"
#include "CowNoWildcardEdit.h"

CCowNoWildcardEdit::CCowNoWildcardEdit()
{
	SetRegEx(L"[A-Za-z]{0,2}[0-9]{0,14}");
}
