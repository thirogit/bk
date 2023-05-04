#include "stdafx.h"
#include "PrintProfile.h"
#include "../../arrays/DeepDelete.h"
#include <boost/range/adaptors.hpp>

PrintProfile::PrintProfile(uint32_t id) : m_profileId(id)
{	
}

PrintProfile::PrintProfile(const PrintProfile& src) : PrintProfile(&src)
{
	
}

PrintProfile::PrintProfile(const IPrintProfile* src) : m_profileId(src->GetProfileId())
{
	CopyFrom(src);
}

PrintProfile::~PrintProfile()
{
	DeepDelete(m_options);
	DeepDelete(m_orderSpecifiers);
}

PrintProfile& PrintProfile::operator=(const PrintProfile& src)
{
	m_profileId = src.m_profileId;
	CopyFrom(&src);
	return *this;
}

void PrintProfile::CopyFrom(const IPrintProfile* src)
{
	DeepDelete(m_options);
	m_options.clear();

	m_profileName = src->GetProfileName();
	m_documentCode = src->GetDocumentCode();

	PtrEnumerator<PrintOption> optionsEnum = src->EnumOptions();
	PrintOption* option;
	while (optionsEnum.hasNext())
	{
		option = *optionsEnum;
		AddOption(new PrintOption(*option));
		optionsEnum.advance();
	}	

	CopyOrderSpecifiers(src);
}

const std::wstring& PrintProfile::GetDocumentCode() const
{
	return m_documentCode;
}

void PrintProfile::SetDocumentCode(const std::wstring& sDocCode)
{
	m_documentCode = sDocCode;
}

count_t PrintProfile::GetOptionsCount() const
{
	return m_options.size();
}

count_t PrintProfile::GetOrderSpecifiersCount() const
{
	return m_orderSpecifiers.size();
}

PtrEnumerator<OrderSpecifier> PrintProfile::EnumOrderSpecifiers() const
{
	return PtrEnumerator<OrderSpecifier>(m_orderSpecifiers);
}

void PrintProfile::SetProfileName(const std::wstring& sName)
{
	m_profileName = sName;
}

uint32_t PrintProfile::GetProfileId() const
{
	return m_profileId;
}

const std::wstring& PrintProfile::GetProfileName() const
{
	return m_profileName;
}

PrintOption* PrintProfile::GetOption(const std::wstring& optName) const
{
	auto optionIt = m_options.find(optName);
	if (optionIt != m_options.end())
		return optionIt->second;

	return NULL;
}

PrintOption* PrintProfile::AddOption(const std::wstring& optName)
{
	PrintOption* newOption = new PrintOption(optName);
	AddOption(newOption);
	return newOption;
}

void PrintProfile::RemoveOption(const std::wstring& optName)
{
	auto optionIt = m_options.find(optName);
	if (optionIt != m_options.end())
	{
		delete optionIt->second;
		m_options.erase(optionIt);
	}
}

PtrEnumerator<PrintOption> PrintProfile::EnumOptions() const 
{
	return PtrEnumerator<PrintOption>( boost::adaptors::values(m_options));
}

void PrintProfile::AddOption(PrintOption* option)
{
	auto optionIt = m_options.find(option->name());
	if (optionIt != m_options.end())
	{
		delete optionIt->second;
		m_options.erase(optionIt);		
	}
	m_options[option->name()] = option;
}

void PrintProfile::AddOption(const PrintOption& option)
{
	AddOption(new PrintOption(option));
}

void PrintProfile::CopyOptions(const IPrintProfile* src)
{
	DeepDelete(m_options);
	m_options.clear();

	PtrEnumerator<PrintOption> enumOption = src->EnumOptions(); 
	while (enumOption.hasNext())
	{
		AddOption(new PrintOption(**enumOption));
		enumOption.advance();
	}
}

void PrintProfile::CopyOrderSpecifiers(const IPrintProfile* src)
{
	ClearOrderSpecifiers();
	PtrEnumerator<OrderSpecifier> specifiersEnum = src->EnumOrderSpecifiers();
	OrderSpecifier* specifier;
	while (specifiersEnum.hasNext())
	{
		specifier = *specifiersEnum;
		AddOrderSpecifier(specifier->fieldCode())->asc(specifier->asc());
		specifiersEnum.advance();
	}
}

OrderSpecifier* PrintProfile::AddOrderSpecifier(const std::wstring& fieldCode)
{
	OrderSpecifier* newSpecifier = new OrderSpecifier(fieldCode);
	m_orderSpecifiers.push_back(newSpecifier);
	return newSpecifier;
}

OrderSpecifier* PrintProfile::GetOrderSpecifierAt(int index)
{
	return m_orderSpecifiers.at(index);
}

void PrintProfile::DeleteOrderSpecifierAt(int index)
{
	auto it = m_orderSpecifiers.begin() + index;
	delete *it;
	m_orderSpecifiers.erase(it);
}

void PrintProfile::ClearOrderSpecifiers()
{
	DeepDelete(m_orderSpecifiers);
	m_orderSpecifiers.clear();
}