#include "stdafx.h"
#include "YesNoBox.h"

bool YesNoBox::Ask(const std::wstring& question)
{
	return AfxMessageBox(question.c_str(),MB_YESNO) == IDYES;
}


