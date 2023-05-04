#include "stdafx.h"
#include "BoolUtil.h"


namespace BoolUtil
{
	bool FromString(const std::wstring& boolstr)
	{
		return (boolstr == L"y" || boolstr == L"Y" || boolstr == L"t" || boolstr == L"1" || boolstr == L"true");
	}

	std::wstring ToString(bool b, BoolFormat format/* = bool_truefalse*/)
	{
		switch (format)
		{
		case bool_yn:
			return b ? L"y" : L"n";
		case bool_tf:
			return b ? L"t" : L"f";
		case bool_yesno:
			return b ? L"yes" : L"no";
		
		}

		return b ? L"true" : L"false";
	}
	
};
