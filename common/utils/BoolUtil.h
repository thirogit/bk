#if !defined(__BOOLUTIL_H__)
#define __BOOLUTIL_H__

#include <string>


namespace BoolUtil
{
	enum BoolFormat
	{
		bool_yn,
		bool_tf,
		bool_yesno,
		bool_truefalse
	};

	bool FromString(const std::wstring& boolstr);	
	std::wstring ToString(bool b, BoolFormat format = bool_truefalse);
};

#endif 
