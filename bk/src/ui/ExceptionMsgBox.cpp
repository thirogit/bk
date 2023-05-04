#include "stdafx.h"
#include "ExceptionMsgBox.h"
#include <codecvt>

void ExceptionMsgBox::TryCatch(std::function<void ()>& f)
{
	try
	{
		f();
	}
	catch (wexception& e)
	{
		Show(e);
	}
	catch(std::exception& e)
	{
		Show(e);
	}
}

void ExceptionMsgBox::Show(std::exception& e)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring msg = converter.from_bytes(e.what());
	AfxMessageBox(msg.c_str(), MB_OK | MB_ICONERROR);
}
	
void ExceptionMsgBox::Show(wexception& e)
{
	AfxMessageBox(e.wwhat(),MB_OK | MB_ICONERROR);
}



