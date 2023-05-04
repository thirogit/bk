#include "stdafx.h"
#include "ErrorToken.h"
#include <iostream>
#include <sstream>
#include <string/TextUtils.h>
#include <string/IntString.h>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <map>


ErrorToken::ErrorToken()
{
}

ErrorToken::ErrorToken(const std::wstring& sErrorCode) : m_sErrorCode(sErrorCode)
{
}

ErrorToken::ErrorToken(const ErrorToken& src) : m_sErrorCode(src.m_sErrorCode)
{
	std::copy(src.m_Tokens.begin(), src.m_Tokens.end(), std::insert_iterator<token_container>(m_Tokens, m_Tokens.begin()));
}

ErrorToken& ErrorToken::operator()(const wchar_t* tokenName, const wchar_t* tokenValue)
{
	Token token;
	token.name = tokenName;
	token.value = tokenValue;
	m_Tokens.insert(token);
	return *this;
}

ErrorToken& ErrorToken::operator()(const wchar_t* tokenName, const std::wstring& tokenValue)
{
	Token token;
	token.name = tokenName;
	token.value = tokenValue;
	m_Tokens.insert(token);
	return *this;
}

ErrorToken& ErrorToken::operator()(const wchar_t* tokenName, uint32_t tokenValue)
{
	Token token;
	token.name = tokenName;
	token.value = IntString(tokenValue);
	m_Tokens.insert(token);
	return *this;
}

std::wstring ErrorToken::str() const
{
	
	std::wstringbuf buffer;
	std::wostream os (&buffer);

	os << m_sErrorCode;
	if(!m_Tokens.empty())
	{
		os	<< L':';

		auto tokenIt = m_Tokens.begin();
		while(tokenIt != m_Tokens.end())
		{
			os << tokenIt->name;
			os << L'=';
			os << TextUtils::Quote(tokenIt->value);
			tokenIt++;
			if (tokenIt != m_Tokens.end())
				os << L',';
		}	
	}
	os.flush();
	return buffer.str();
}

const std::wstring& ErrorToken::code() const
{
	return m_sErrorCode;
}

namespace qi = boost::spirit::qi;
namespace ns = boost::spirit::standard_wide;
namespace phoenix = boost::phoenix;


ErrorToken ErrorToken::tokenize(const std::wstring& sErrorToken)
{
	typedef std::wstring::const_iterator Iterator;
	using boost::spirit::qi::double_;
	using boost::spirit::qi::_1;
	using boost::spirit::qi::phrase_parse;
	using ns::space;

	using boost::spirit::qi::lit;
	using boost::spirit::qi::int_;
	using qi::lexeme;
	using ns::char_;

	std::wstring errorCode;
	std::map<std::wstring, std::wstring> tokensMap;
	
	qi::rule<Iterator, std::wstring(), ns::space_type> errorCodeLexeme;
	qi::rule<Iterator, std::pair<std::wstring, std::wstring>(), ns::space_type> token;
	qi::rule<Iterator, std::map<std::wstring, std::wstring>(), ns::space_type> tokens;
	qi::rule<Iterator, std::wstring(), ns::space_type> name, value;
	qi::rule<Iterator, std::wstring(), ns::space_type> quoted_string;

	errorCodeLexeme %= lexeme[+(char_ - L':')];

	quoted_string %= lit(L'\'') >> +(char_ - L'\'') >> L'\'';
	name %= char_(L"a-zA-Z_") >> *char_(L"a-zA-z_0-9");
	value %= +char_;
	token %= name >> lit(L'=') >> quoted_string;
	tokens %= token >> *(lit(L',') >> token);

	Iterator first = sErrorToken.begin();
	Iterator last = sErrorToken.end();

	bool r = phrase_parse(first, last,

		//  Begin grammar
		(
		errorCodeLexeme[phoenix::ref(errorCode) = _1] >> -(L':' >> tokens[phoenix::ref(tokensMap) = _1])
		),
		//  End grammar

		space);

	if (!r || first != last)
		BOOST_THROW_EXCEPTION(ErrorTokenException(L"bad token"));


	ErrorToken result;
	result.m_sErrorCode = errorCode;

	auto it = tokensMap.begin();
	while (it != tokensMap.end())
	{
		Token token;
		token.name = it->first;
		token.value = it->second;
		result.m_Tokens.insert(token);
		it++;
	}
	
	return std::move(result);
		
}

ErrorToken::const_iterator ErrorToken::begin() const
{
	return m_Tokens.begin();
}

ErrorToken::const_iterator ErrorToken::end() const
{
	return m_Tokens.end();
}

ErrorToken::iterator ErrorToken::begin()
{
	return m_Tokens.begin();
}

ErrorToken::iterator ErrorToken::end()
{
	return m_Tokens.end();
}