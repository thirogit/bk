#include "stdafx.h"
#include "TypesCompare.h"
#include <cstring>
#include <boost/algorithm/string.hpp>

namespace TypesCompare
{

	COMPARE_RESULT TrimResult(int result)
	{
		if (result < 0) return COMPARE_LESS;
		else if (result) return COMPARE_GREATHER;

		return COMPARE_EQUAL;
	}

	COMPARE_RESULT CompareDateTime(const DateTime &left,const DateTime &right)
	{
		return CompareNullable<DateTime>(left,right);
	}
	
	COMPARE_RESULT CompareNullInt(const NullInt &left,const NullInt &right)
	{
		return CompareNullable<NullInt>(left,right);
	}

	COMPARE_RESULT CompareInt(const int left,const int right)
	{
		
		if(left > right)
			return COMPARE_GREATHER;
		else
		{
			if(left < right)
				return COMPARE_LESS;
		}
		return COMPARE_EQUAL;
	}


	COMPARE_RESULT CompareLocaleString(const std::wstring &str1,const std::wstring &str2)
	{
		if (boost::algorithm::is_less()(str1,str2))
			return COMPARE_LESS;
		else
		{
			if (boost::algorithm::is_less()(str2, str1))
				return COMPARE_GREATHER;
		}
		return COMPARE_EQUAL;
	}

	COMPARE_RESULT CompareBool(bool left,bool right)
	{	
		if(left > right)
					return COMPARE_GREATHER;
		else
		{
			if(left == right)
					return COMPARE_EQUAL;
		}
		
		return COMPARE_LESS;
	}
	COMPARE_RESULT CompareAnsiString(const std::string& left,const std::string& right)
	{
		return left.compare(right);
	}

}

