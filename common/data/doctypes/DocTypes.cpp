#include "stdafx.h"
#include "DocTypes.h"
#include <boost/algorithm/string.hpp>

namespace DocTypes
{

	DocType GetType(char cType)
	{
		switch ((DocType)cType)
		{
		case DocType_In:
		case DocType_Buy:
		case DocType_Move:
		case DocType_Sell:
		case DocType_Out:
			return (DocType)cType;
		}
		return DocType_None;
	}

	DocType GetType(const std::wstring& sType)
	{
		if (sType.length() == 1)
		{
			return GetType(sType[0]);
		}
		else
		{
			if (boost::iequals(L"in", sType))
				return DocType_In;

			if (boost::iequals(L"buy", sType))
				return DocType_Buy;

			if (boost::iequals(L"move", sType))
				return DocType_Move;

			if (boost::iequals(L"sell", sType))
				return DocType_Sell;

			if (boost::iequals(L"out", sType))
				return DocType_Out;
		}
		return DocType_None;
	}

	std::wstring ToString(DocType type)
	{
		switch (type)
		{
			case DocType_In:
				return L"in";
			case DocType_Buy:
				return L"buy";
			case DocType_Move:
				return L"move";
			case DocType_Sell:
				return L"sell";
			case DocType_Out:
				return L"out";				
		}

		return L"";
	}

}

