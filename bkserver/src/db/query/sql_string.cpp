#include "stdafx.h"
#include "sql_string.h"
#include <sstream>
#include <data/IIdable.h>

using namespace std;

SQLString::SQLString(const SQLString &copy) : is_string(copy.is_string), dont_escape(copy.dont_escape), processed(copy.processed),
											  zerolen2null(copy.zerolen2null),wstring(copy)
{
	
}

SQLString::SQLString() : is_string(false), dont_escape(false), processed(false), zerolen2null(false)
{
}

SQLString::SQLString(const wstring& str,bool zerolen) : wstring(str), is_string(true), dont_escape(false), processed(false), zerolen2null(zerolen)
{
}

/*
SQLString::SQLString(const char* str,bool zerolen) : 
string(str),
is_string(true),
dont_escape(false),
processed(false),
zerolen2null(zerolen)
{
}*/

SQLString::SQLString(wchar_t ch) : is_string(true), dont_escape(false), processed(false)
{
	wostringstream outs;
	outs << ch;
	assign(outs.str());
}


SQLString::SQLString(short int i) : is_string(false), dont_escape(false), processed(false)
{
	wostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(unsigned short int i) : is_string(false), dont_escape(false), processed(false)
{
	wostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(int i) : is_string(false), dont_escape(false), processed(false)
{
	wostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(unsigned int i) : is_string(false), dont_escape(false), processed(false)
{
	wostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(longlong i) : is_string(false), dont_escape(false), processed(false)
{
	wostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(ulonglong i) : is_string(false), dont_escape(false), processed(false) 
{
	wostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(float f) : is_string(false), dont_escape(false), processed(false)
{
	AssignDouble((double)f);
}

SQLString::SQLString(double f) : is_string(false), dont_escape(false), processed(false)
{
	AssignDouble(f);
}

void SQLString::AssignDouble(double d)
{
	wostringstream outs;
	outs.precision(STREAM_FLOAT_PREC);
	outs << d;
	assign(outs.str());
}

SQLString::SQLString(const NullInt &nullint) : is_string(false), dont_escape(false), processed(false)
{
	wostringstream outs;
	if(nullint.IsNull())
	 outs << L"NULL";
	else outs << nullint.ToInt();
	assign(outs.str());
}

SQLString::SQLString(const AccountNo& accountNo) : is_string(true), dont_escape(false), processed(false)
{
	wostringstream outs;
	if(accountNo.IsNull())
	{
		outs << L"NULL";		
		is_string = false;
	}
	else
	{
		outs << accountNo.ToString();
	}
	assign(outs.str());
}

SQLString::SQLString(const HentNo& hentNo) : is_string(true),dont_escape(false),processed(false)
{
	wostringstream outs;
	if(hentNo.IsNull())
	{
		outs << L"NULL";
		is_string = false;
	}
	else
	{
		outs << hentNo.ToString();
	}
	assign(outs.str());
}

SQLString::SQLString(const CowNo& cowNo): is_string(true),dont_escape(false),processed(false)
{
	wostringstream outs;
	if(cowNo.IsNull())
	{
		outs << L"NULL";
		is_string = false;
	}
	else
	{
		outs << cowNo.ToString();
	}
	assign(outs.str());
}

SQLString::SQLString(const NullDouble &nulldouble) : is_string(false), dont_escape(false), processed(false)
{
	wostringstream outs;
	if(nulldouble.IsNull())
	{
		outs << L"NULL";
		is_string = false;
		assign(outs.str());
	}
	else
	{
		AssignDouble(nulldouble.GetDouble());
	}
}

SQLString::SQLString(const CowSex &cowsex) : is_string(false), dont_escape(false), processed(false)
{
	wostringstream outs;
	outs << (int)cowsex.getCOWSEX();
	assign(outs.str());
}

SQLString::SQLString(const DateTime &datetime,bool bShortDate/* = false*/) : dont_escape(false), processed(false), is_string(false)
{
	wostringstream outs;
	if(!datetime.IsNull())
	{	
		
		std::wstring sDt; 
		if(bShortDate)
			sDt = datetime.GetDateInFmt(DateTime::YYYYdotMMdotDD);
		else
			sDt = datetime.GetDateInFmt(DateTime::YYYYdotMMdotDD_HHcolonMMcolonSS);
		outs << L'\'' << sDt << L'\'';
	}
	else
	{		
		outs << L"NULL";
	}
	assign(outs.str());
}

SQLString::SQLString(const Decimal& decimal) : is_string(false),dont_escape(true),processed(false)
{
	wostringstream outs;	
	outs << decimal.ToString(DecimalFormat(5,L'.'));	
	assign(outs.str());
}

SQLString::SQLString(const NullDecimal& decimal)
{
	wostringstream outs;
	if(decimal.IsNull()) outs << L"NULL";
	else outs << decimal.ToString(DecimalFormat(5,L'.'));	
	assign(outs.str());
}

SQLString::SQLString(const Time& time) : is_string(false),dont_escape(true),processed(false)
{
	wostringstream outs;	
	if(time.IsNull()) outs << L"NULL";
	else outs << time.GetSecsFromMidnight();
	assign(outs.str());
}

SQLString& SQLString::operator=(const wchar_t* str)
{
	std::wstring::operator =(str);
	return *this;
}

	
SQLString& SQLString::operator=(const std::wstring& str)
{
	std::wstring::operator =(str);
	return *this;
}

/*
std::wstring SQLString::quotedstr(void)
{
	if(!length() && zerolen2null)
	{	
		return (std::wstring)"NULL";
	}
	if (is_string) 
	{
		if (!dont_escape) 
		{
			char* s = new char[size() * 2 + 1];
			escape_string(s, c_str(), size());
			std::string ret = s;	
			delete []s;
			return '\'' + ret + '\'';
		}			
	
	}
	return  *this;
}*/

//-------------------------------------------------------------------------------------------------

SQLStringNull::SQLStringNull(): SQLString(L"NULL")
{
	is_string = false;
}

SQLIdable::SQLIdable(uint32_t id)
{
	is_string = false;

	wostringstream outs;

	if(id == NULL_ID) outs << L"NULL";
	else outs << id;

	assign(outs.str());
}

