#include "stdafx.h"
#include "InvoiceExporterOptions.h"
#include <arrays/DeepDelete.h>
#include <boost/range/adaptors.hpp>


InvoiceExporterOptions::InvoiceExporterOptions()
{

}

InvoiceExporterOptions::InvoiceExporterOptions(const InvoiceExporterOptions& src) 
{
	
}

InvoiceExporterOptions::~InvoiceExporterOptions()
{
	DeepDelete(m_options);
}

InvoiceExporterOptions& InvoiceExporterOptions::operator=(const InvoiceExporterOptions& src)
{
	return *this;
}

count_t InvoiceExporterOptions::GetOptionsCount() const
{
	return m_options.size();
}


InvoiceExporterOption* InvoiceExporterOptions::GetOption(const std::wstring& optName) const
{
	auto optionIt = m_options.find(optName);
	if (optionIt != m_options.end())
		return optionIt->second;

	return NULL;
}

InvoiceExporterOption* InvoiceExporterOptions::AddOption(const std::wstring& optName)
{
	InvoiceExporterOption* newOption = new InvoiceExporterOption(optName);
	AddOption(newOption);
	return newOption;
}

void InvoiceExporterOptions::RemoveOption(const std::wstring& optName)
{
	auto optionIt = m_options.find(optName);
	if (optionIt != m_options.end())
	{
		delete optionIt->second;
		m_options.erase(optionIt);
	}
}

PtrEnumerator<InvoiceExporterOption> InvoiceExporterOptions::EnumOptions() const
{
	return PtrEnumerator<InvoiceExporterOption>( boost::adaptors::values(m_options));
}

void InvoiceExporterOptions::AddOption(InvoiceExporterOption* option)
{
	auto optionIt = m_options.find(option->name());
	if (optionIt != m_options.end())
	{
		delete optionIt->second;
		m_options.erase(optionIt);		
	}
	m_options[option->name()] = option;
}

void InvoiceExporterOptions::AddOption(const InvoiceExporterOption& option)
{
	AddOption(new InvoiceExporterOption(option));
}

