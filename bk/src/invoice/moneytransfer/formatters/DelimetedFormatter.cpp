#include "stdafx.h"
#include "DelimetedFormatter.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include <string\TextUtils.h>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>

#include <locale>
#include <codecvt>
#include <iostream>


DelimetedFormatter::DelimetedFormatter(wchar_t cDelimiter,const std::wstring& sExtension,const std::wstring& sFormatterName) : 
	m_cDelimiter(cDelimiter),m_sExtension(sExtension),m_sFormatterName(sFormatterName)
{
}

DelimetedFormatter::~DelimetedFormatter()
{
	DeepDelete(m_fields);
}

void DelimetedFormatter::AddField(FormatterField* pField)
{
	m_fields.push_back(pField);
}

std::wstring DelimetedFormatter::GetFormatterName()
{
	return m_sFormatterName;
}

std::wstring DelimetedFormatter::GetFileExtension()
{
	return m_sExtension;
}

void DelimetedFormatter::FormatTransfers(MoneyTransferEntryRange moneyTransfers,
											  MoneyTransferContext* pContext,
											  std::wofstream& output,
											  SimpleProgress *progress)
{
	std::wstring sFieldValue;	
	progress->SetSteps(moneyTransfers.size());

	BOOST_FOREACH(MoneyTransferEntry* pMoneyTransfer,moneyTransfers)
	{
		auto fieldIt = m_fields.begin();
		while(fieldIt != m_fields.end())
		{
			FormatterField* pField = *(fieldIt++);		
			
			sFieldValue = pField->GetFieldValue(pContext, pMoneyTransfer);			
			//std::string utf8FieldValue = TextUtils::ToUTF8(sFieldValue);
			output << sFieldValue;
			if (fieldIt != m_fields.end())
			{
				WriteDelimiter(output);
				
			}
		}		
		WriteLineEnd(output);		
		if(!progress->Step())
			return;
	}
}

void DelimetedFormatter::WriteLineEnd(std::wofstream& output)
{
	output << L"\r\n";
}

void DelimetedFormatter::WriteDelimiter(std::wofstream& output)
{
	output << m_cDelimiter;
}

std::string DelimetedFormatter::GetDefaultEncoding()
{
	return "UTF-8";
}