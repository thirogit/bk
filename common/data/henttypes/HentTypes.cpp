#include "stdafx.h"
#include "HentTypes.h"
#include <boost/algorithm/string.hpp>

namespace HentTypes
{

	HentType FromChar(wchar_t cHentType)
	{
		if (HentType_Individual == (HentType)cHentType)
			return HentType_Individual;

		if (HentType_Company == (HentType)cHentType)
			return HentType_Company;

		return HentType_None;
	}

	

	std::wstring ToString(HentType henttype)
	{
		switch (henttype)
		{
			case HentType_Company:
				return L"company";
			case HentType_Individual:
				return L"individual";
		}

		return L"";
	}

	HentType FromString(const std::wstring& type)
	{
		if (type.length() == 1)
		{
			return FromChar(type[0]);
		}
		else
		{
			if (boost::iequals(L"company", type))
				return HentType_Company;

			if (boost::iequals(L"individual", type))
				return HentType_Individual;
		}
		return HentType_None;

	}

}

