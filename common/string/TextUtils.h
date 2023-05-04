#ifndef __TEXTUTILS_H__
#define __TEXTUTILS_H__

#include <string>
#include <cstdarg>
#include <boost/format.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/for_each.hpp>
#include "../exception/TagException.h"

namespace TextUtils
{
	struct tag_AlphaToFloatException{};
	typedef TagException<tag_AlphaToFloatException> AlphaToFloatException;

	double AlphaToFloat(const std::wstring& str, wchar_t decimalPt = L'.');
	bool IsNumber(const std::wstring& str);
	bool IsAlpha(const std::wstring& str);
	std::wstring Right(const std::wstring& s,int chars);
	std::wstring Left(const std::wstring& s,int chars);
	bool startsWith(const std::wstring& s, const std::wstring& prefix);
	std::wstring FormatV(const wchar_t* szFmt,va_list args);
	std::wstring FormatA(const wchar_t* szFmt,...);
	std::wstring FormatA(const std::wstring& sFmt,...);
	std::wstring Quote(const std::wstring& s);
	std::wstring Remove(const std::wstring& s,wchar_t c);
	bool IsAllWhitespace(const std::wstring& s);
	std::wstring RemoveWhitespace(const std::wstring& s);
	std::string ToUTF8(const std::wstring& s);
	std::wstring FromUTF8(const std::string& s);
	std::wstring PadBegin(const std::wstring& s, wchar_t padChar, int requiredLength);
	std::wstring JoinLines(const std::wstring& s, wchar_t joinChar);
	bool ContainsNoCase(const std::wstring& s, const std::wstring& part);
	std::wstring IfEmpty(const std::wstring& s,const std::wstring& ifEmpty);
	bool IsOneOf(char v, char c1, char c2, char c3);
	bool IsOneOf(char v, char c1, char c2, char c3, char c4);

	struct FormatAppender
	{
		FormatAppender() : delegate(NULL) {}

		template <class T>
		void operator()(const T& t) const
		{
			delegate->operator%(t);
		}

		boost::wformat* delegate;
	};
	
	template<typename PARAMETERS>
	std::wstring FormatFusion(const std::wstring& sFmt,PARAMETERS& parameters)
	{
		boost::wformat fmter(sFmt);

		FormatAppender appender;
		appender.delegate = &fmter;

		boost::fusion::for_each(parameters,appender);
		
		return fmter.str();
	}

	template<typename A1>
	std::wstring Format(const std::wstring& sFmt,A1 a)
	{
		return (boost::wformat(sFmt) % a).str();
	}

	template<typename A1,typename A2>
	std::wstring Format(const std::wstring& sFmt,A1 a1,A2 a2)
	{
		boost::fusion::vector2<A1,A2> parameters(a1,a2); 
		return FormatFusion(sFmt,parameters);
	}

	template<typename A1,typename A2,typename A3>
	std::wstring Format(const std::wstring& sFmt,A1 a1,A2 a2,A3 a3)
	{
		boost::fusion::vector3<A1,A2,A3> parameters(a1,a2,a3); 
		return FormatFusion(sFmt,parameters);
	}

	template<typename A1, typename A2, typename A3, typename A4>
	std::wstring Format(const std::wstring& sFmt, A1 a1, A2 a2, A3 a3,A4 a4)
	{
		boost::fusion::vector4<A1, A2, A3, A4> parameters(a1, a2, a3, a4);
		return FormatFusion(sFmt, parameters);
	}

	template<typename A1, typename A2, typename A3, typename A4, typename A5>
	std::wstring Format(const std::wstring& sFmt, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		boost::fusion::vector5<A1, A2, A3, A4, A5> parameters(a1, a2, a3, a4, a5);
		return FormatFusion(sFmt, parameters);
	}

	template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	std::wstring Format(const std::wstring& sFmt, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5,A6 a6)
	{
		boost::fusion::vector6<A1, A2, A3, A4, A5, A6> parameters(a1, a2, a3, a4, a5, a6);
		return FormatFusion(sFmt, parameters);
	}

}

std::wstring w(const std::string& s);
std::string u(const std::wstring& s);

#endif
