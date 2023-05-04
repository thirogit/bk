#include "stdafx.h"
#include "HourMinuteEdit.h"
#include "../../utils/StringLoader.h"
#include "../../resource.h"

CHourMinuteEdit::CHourMinuteEdit()
{
	SetMask(L"##:##");
}

Time CHourMinuteEdit::GetTime() const
{
	Time t;
	t.MakeNull();
	if(IsInputEmpty()) return t;
	if(!IsFullPrompt()) return t;
	
	std::wstring sTime = GetText();
	t = Time::ParseTime(Time::HHcolonMI, sTime);
	
	return t;
}

void CHourMinuteEdit::SetTime(const Time& t)
{
	SetText(t.GetTime(Time::HHcolonMI));
}

bool CHourMinuteEdit::IsValidTime()
{
	if(IsInputEmpty()) return true;

	try
	{
		Time t = GetTime();
		return !t.IsNull();
	}
	catch (...)
	{
		return false;
	}	
}

BOOL CHourMinuteEdit::OnValidate()
{
	if(!COXMaskedEdit::OnValidate()) return FALSE;

	if(!IsValidTime())
	{
		SetToolTipText(StringLoader(IDS_ERRASKFORVALIDHHMM).c_str());
		SetToolTipTitle(TTI_ERROR,StringLoader(IDS_ERRINVALIDTIME).c_str());
		ShowToolTip();		
		return FALSE;
	}
	return TRUE;
}

