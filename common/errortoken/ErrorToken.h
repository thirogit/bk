#pragma once

#include <string>
#include <cstdint>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include "../exception/TagException.h"

struct Token
{
	std::wstring name;
	std::wstring value;
};

struct tag_ErrorTokenException {};
typedef TagException<tag_ErrorTokenException> ErrorTokenException;

class ErrorToken
{
public:
	struct index_tag_name{};

	typedef boost::multi_index_container
		<
		Token,
		boost::multi_index::indexed_by< boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_name>, boost::multi_index::member<Token,std::wstring,&Token::name>> >
		> token_container;

	typedef token_container::const_iterator const_iterator;
	typedef token_container::iterator iterator;


	ErrorToken(const ErrorToken& src);

	ErrorToken(const std::wstring& sErrorCode);
	ErrorToken& operator()(const wchar_t* tokenName, const wchar_t* tokenValue);
	ErrorToken& operator()(const wchar_t* tokenName, const std::wstring& tokenValue);
	ErrorToken& operator()(const wchar_t* tokenName, uint32_t tokenValue);
	std::wstring str() const;

	const std::wstring& code() const;

	static ErrorToken tokenize(const std::wstring& sErrorToken);

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();
protected:
	ErrorToken();
private:
	std::wstring m_sErrorCode;
	token_container m_Tokens;
};