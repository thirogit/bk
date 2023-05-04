#ifndef __TYPESCOMPARE_H__
#define __TYPESCOMPARE_H__

#include <types/DateTime.h>
//#include <types/Money.h>
#include <types/NullInt.h>

#include "CompareDefs.h"
#include <string>

namespace TypesCompare
{
	template<typename U>
	COMPARE_RESULT CompareNullable(const U &left,const U &right)
	{

		if(!left.IsNull())
		{
			if(!right.IsNull())
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
			return COMPARE_GREATHER;
		}
		else
		{
			if(!right.IsNull())
					return COMPARE_LESS;
		}
		return COMPARE_EQUAL;
	}
	
	template<typename TYPE>
	COMPARE_RESULT CompareType(const TYPE& left,const TYPE& right)
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


	COMPARE_RESULT CompareDateTime(const DateTime &left,const DateTime &right);
	//COMPARE_RESULT CompareMoney(const Money &left,const Money &right);
	COMPARE_RESULT CompareNullInt(const NullInt &left,const NullInt &right);
	COMPARE_RESULT CompareInt(const int left,const int right);
	//COMPARE_RESULT CompareNullDouble(const NullDouble& left,const NullDouble& right);
	COMPARE_RESULT CompareLocaleString(const std::wstring &str1,const std::wstring &str2);
	COMPARE_RESULT CompareBool(bool left,bool right);
	//COMPARE_RESULT CompareAnsiString(const std::string& left,const std::string& right);

}

#endif