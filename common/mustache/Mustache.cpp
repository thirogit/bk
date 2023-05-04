#include "stdafx.h"
#include "Mustache.h"
#include "..\arrays\DeepDelete.h"
#include <boost/foreach.hpp>
#include <boost\range\adaptors.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include "../string/TextUtils.h"

typedef std::tuple<std::wstring, bool, int > mustache_variable;

namespace mustache_parser
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;

	template <typename Iterator>
	bool parse_complex(Iterator first, Iterator last, mustache_variable& varaible)
	{
		using boost::spirit::qi::double_;
		using boost::spirit::qi::_1;
		using boost::spirit::qi::phrase_parse;
		using boost::spirit::ascii::space;

		using boost::spirit::qi::lit;
		using boost::spirit::qi::int_;
		using qi::lexeme;
		using ascii::char_;

		std::wstring varableName;
		bool pad0 = false;
		int padding = 0;

		qi::rule<Iterator, std::wstring(), ascii::space_type> varableNameLexeme;

		varableNameLexeme %= lexeme[+(char_ - L':')];

		bool r = phrase_parse(first, last,

			//  Begin grammar
			(
			varableNameLexeme[phoenix::ref(varableName) = _1] >> -(L':' >> +lit(L'0')[phoenix::ref(pad0) = true] >> int_[phoenix::ref(padding) = _1])
			//varableNameLexeme[boost::phoenix::ref(varableName) = _1] >> -(':' >> int_[boost::phoenix::ref(padding) = _1])
			),
			//  End grammar

			space);

		if (!r || first != last) // fail if we did not get a full match
			return false;

		varaible = std::make_tuple(varableName, pad0, padding);

		return r;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Mustache::Mustache() : m_Slices(new MustacheSlices())
{	
}

Mustache::Mustache(const Mustache& src)
{
	m_Slices = src.m_Slices;
}

Mustache::~Mustache()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheVariable::MustacheVariable(const MustacheVariable& src)
{
	m_sVariableValue = src.m_sVariableValue;
	m_sVariableName = src.m_sVariableName;
}

MustacheVariable::MustacheVariable(const std::wstring& sVariableName, const std::wstring& sValue) :
	m_sVariableName(sVariableName),m_sVariableValue(sValue)
{
}

const std::wstring& MustacheVariable::GetValue() const
{
	return m_sVariableValue;
}

const std::wstring& MustacheVariable::GetName() const
{
	return m_sVariableName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheVariables::MustacheVariables()
{
}

MustacheVariables::~MustacheVariables()
{
	Clear();
}

void MustacheVariables::Clear()
{
	DeepDelete(m_VariablesMap);
	m_VariablesMap.clear();
}


MustacheVariables& MustacheVariables::Put(const MustacheVariable& variable)
{
	std::wstring sVariableName = variable.GetName();

	MustacheVariable* existringVariable = NULL;

	auto it = m_VariablesMap.find(sVariableName);
	if (it != m_VariablesMap.end())
	{
		delete it->second;
	}

	m_VariablesMap[sVariableName] = new MustacheVariable(variable);

	return *this;
}


MustacheVariables& MustacheVariables::Put(const std::wstring& variableName, const std::wstring& variableValue)
{
	return Put(MustacheVariable(variableName, variableValue));
}

const MustacheVariable* MustacheVariables::Get(const std::wstring& sVariableName) const
{	
	MustacheVariable* pVariable = NULL;
	auto it = m_VariablesMap.find(sVariableName);
	if (it != m_VariablesMap.end()) pVariable = it->second;
	return pVariable;
}

MustacheVariables::MustacheVariables(const MustacheVariables& src)
{
	BOOST_FOREACH(MustacheVariable* variable, src.m_VariablesMap | boost::adaptors::map_values)
	{
		m_VariablesMap[variable->GetName()] = new MustacheVariable(*variable);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MustacheVariablesValueResolver::MustacheVariablesValueResolver(const MustacheVariables& variables) : m_variables(variables)
{
}

std::wstring MustacheVariablesValueResolver::Get(const std::wstring& sVariableName) const
{
	const MustacheVariable* variable = m_variables.Get(sVariableName);
	if (variable != NULL)
	{
		return variable->GetValue();
	}
	return L"";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MustacheStringStream::MustacheStringStream(const std::wstring& str) : m_String(str), m_Position(0)
{

}

bool MustacheStringStream::IsOneOf(wchar_t c,wchar_t *chs)
{
	wchar_t* characterSet = chs;
	while(*characterSet != 0)
	{
		if(*characterSet == c)
			return true;
		characterSet++;
	}

	return false;
}

std::wstring MustacheStringStream::ReadUntil(wchar_t* stopCharacters)
{
	std::wstring result;
	wchar_t c;
	while(m_Position < m_String.length())
	{
		c = m_String[m_Position];
		if(!IsOneOf(c,stopCharacters))
		{
			result += c;
			m_Position++;
		}
		else
		{
			break;
		}
	}
	return result;
}


wchar_t MustacheStringStream::GetChar()
{
	if(m_Position < m_String.length())
	{
		return m_String[m_Position++];
	}
	return L'\0';
}

wchar_t MustacheStringStream::LookAhead()
{
	if(m_Position+1 < m_String.length())
	{
		return m_String[m_Position];
	}
	return L'\0';
}

bool MustacheStringStream::IsEnd() const
{
	return !(m_Position < m_String.length());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheVariableSlice::MustacheVariableSlice(const std::wstring& sSliceVariableName, int padLength, wchar_t padChar) : 
	m_SliceVariableName(sSliceVariableName), m_padLength(padLength), m_padChar(padChar)
{
}

std::wstring MustacheVariableSlice::GetSliceValue(MustacheValueResolver* resolver)
{
	std::wstring value = resolver->Get(m_SliceVariableName);
	if (m_padLength > 0)
	{
		return TextUtils::PadBegin(value, (m_padChar != 0 ? m_padChar : L' '), m_padLength);
	}
	return value;
}

MustacheVariableSlice::~MustacheVariableSlice()
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheConstantSlice::MustacheConstantSlice(const std::wstring& sSliceValue) : m_SliceValue(sSliceValue)
{
}

std::wstring MustacheConstantSlice::GetSliceValue(MustacheValueResolver* resolver)
{
	return m_SliceValue;
}

MustacheConstantSlice::~MustacheConstantSlice()
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheSlices::MustacheSlices()
{
}

MustacheSlices::~MustacheSlices()
{
	DeepDelete(m_Slices);
}

void MustacheSlices::AddSlice(MustacheSlice* pSlice)
{
	m_Slices.push_back(pSlice);
}

int MustacheSlices::GetSliceCount() const
{
	return m_Slices.size();
}

MustacheSlice* MustacheSlices::GetSlice(int iIndex)
{
	return m_Slices[iIndex];
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MustacheVariableSlice* Mustache::CreateVariable(const std::wstring& variableToken)
{
	mustache_variable variable;
	if (mustache_parser::parse_complex(variableToken.begin(), variableToken.end(), variable))
	{
		return new MustacheVariableSlice(std::get<0>(variable), std::get<2>(variable), std::get<1>(variable) ? L'0' : L' ');
	}

	return new MustacheVariableSlice(variableToken,-1,0);
}

Mustache Mustache::Parse(const std::wstring& str)
{
	Mustache context;

	MustacheStringStream stream(str);
	wchar_t c,nextc;
	std::wstring variableToken, sSlice;

	while(!stream.IsEnd())
	{
		sSlice += stream.ReadUntil(L"\\{");
		c = stream.GetChar();
		if(c != L'\0')
		{
			switch(c)
			{
				case L'{':
					if(!sSlice.empty())
					{
						context.m_Slices->AddSlice(new MustacheConstantSlice(sSlice));
						sSlice.clear();
					}

					variableToken = stream.ReadUntil(L"}");
					nextc = stream.GetChar();
					if(nextc != L'}')
					{
						BOOST_THROW_EXCEPTION(MustacheException(L"missing closing bracket"));
					}
					else
					{
						context.m_Slices->AddSlice(CreateVariable(variableToken));
					}
				break;

				case L'\\':
					nextc = stream.GetChar();

					if (nextc = L'n')
					{
						sSlice += L'\n';
						break;
					}

					if(nextc != L'\0' && (nextc == L'\\' || nextc == L'{'))
					{
						sSlice += nextc;
					}
					else
					{
						BOOST_THROW_EXCEPTION(MustacheException(TextUtils::Format(L"invalid escape sequence '\\%c' in '%s'",nextc,str)));
					}
				break;

			}
		}
		else
		{
			if(!sSlice.empty())
			{
				context.m_Slices->AddSlice(new MustacheConstantSlice(sSlice));					
				sSlice.clear();
			}

		}
	}
	return context;
}

std::wstring Mustache::Format(const MustacheVariables& variables)
{
	MustacheVariablesValueResolver resolver(variables);
	return Format(&resolver);
}

std::wstring Mustache::Format(MustacheValueResolver* resolver)
{
	std::wstring sResult;

	MustacheSlice* pSlice = NULL;
	for (int iSlice = 0, sliceCount = m_Slices->GetSliceCount(); iSlice<sliceCount; iSlice++)
	{
		pSlice = m_Slices->GetSlice(iSlice);
		sResult += pSlice->GetSliceValue(resolver);
	}
	return sResult;
}
