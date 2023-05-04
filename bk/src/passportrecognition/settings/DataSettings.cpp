#include "stdafx.h"
#include "DataSettings.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DataSettings::DataSettings()
{
	m_bDoUppercaseHent = true;
	m_bDoNotUpdateValueIfPresent = true;

	m_Fields[PASSPORT_DATE] = true;
	m_Fields[PASSPORT_NO] = true;
	m_Fields[BIRTH] = true;
	m_Fields[SEX] = true;
	m_Fields[STOCK] = true;
	m_Fields[MOTHER_NO] = true;
	m_Fields[FIRST_OWNER] = true;
}
	
	
bool DataSettings::GetDoUppercaseHent() const
{
	return m_bDoUppercaseHent;
}

bool DataSettings::GetDoNotUpdateValueIfPresent() const
{
	return m_bDoNotUpdateValueIfPresent;
}


bool DataSettings::IsFieldEnabled(DATA_FIELD optinalField) const
{
	std::map<DATA_FIELD, bool>::const_iterator it = m_Fields.find(optinalField);
	if(it != m_Fields.end())
	{
		return it->second;
	}
	return false;
}
	
void DataSettings::SetDoUppercaseHent(bool bDoUppercaseHent)
{
	m_bDoUppercaseHent = bDoUppercaseHent;
}

void DataSettings::SetDoNotUpdateValueIfPresent(bool bDoNotUpdateValueIfPresent)
{
	m_bDoNotUpdateValueIfPresent = bDoNotUpdateValueIfPresent;
}

void DataSettings::SetOptionalFieldEnabled(DATA_FIELD field, bool bEnabled)
{
	m_Fields[field] = bEnabled;
}
