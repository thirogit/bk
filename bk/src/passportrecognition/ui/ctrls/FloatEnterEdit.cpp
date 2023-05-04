#include "stdafx.h"
#include "FloatEnterEdit.h"
#include <boost/format.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FloatEnterEdit::FloatEnterEdit(int prec) : CEnterEdit()
{

	std::wstring sRegEx = L"[0-9]{1,6}";
	if(prec > 0)
	{
		sRegEx += boost::str(boost::wformat(L"([,.][0-9]{0,%d})?") % prec);
	}

	SetRegEx(sRegEx.c_str());

}
