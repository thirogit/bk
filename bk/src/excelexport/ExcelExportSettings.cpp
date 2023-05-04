#include "stdafx.h"
#include "ExcelExportSettings.h"
#include <list>
#include <boost/foreach.hpp>

ExcelExportSettings::ExcelExportSettings() 
{
}

ExcelExportSettings::ExcelExportSettings(const ExcelExportSettings& src)
{
	operator=(src);
}

ExcelExportSettings::ExcelExportSettings(ExcelExportSettings&& src)
{
	m_FieldOrder.swap(src.m_FieldOrder);
}

ExcelExportSettings& ExcelExportSettings::operator=(const ExcelExportSettings& right)
{	
	m_FieldOrder = right.m_FieldOrder;
	return *this;
}

int ExcelExportSettings::GetFieldCount() const
{
	return m_FieldOrder.size();
}

uint32_t ExcelExportSettings::GetField(int nIndex) const
{
	return m_FieldOrder[nIndex];
}

void ExcelExportSettings::AddField(uint32_t fieldId)
{
	m_FieldOrder.push_back(fieldId);
}

bool ExcelExportSettings::HasField(uint32_t fieldId) const
{
	const FieldIndex::nth_index<1>::type& index = m_FieldOrder.get<1>();		
	return index.find(fieldId) != index.end();
}

void ExcelExportSettings::RemoveField(int nIndex)
{
	m_FieldOrder.erase(m_FieldOrder.begin() + nIndex);
}

void ExcelExportSettings::RemoveAllFields()
{
	m_FieldOrder.clear();
}
	
ExcelExportSettings ExcelExportSettings::Load(UserSettings* settings, const std::wstring& settingName)
{
	std::vector<int> orderedEnabledFields = settings->GetSetting<std::vector<int>>(settingName,std::vector<int>());

	ExcelExportSettings result;

	BOOST_FOREACH(int fieldId, orderedEnabledFields)
	{
		result.AddField(fieldId);
	}

	return std::move(result);
}

void ExcelExportSettings::Save(UserSettings* settings, const std::wstring& settingName)
{
	std::vector<int> orderedEnabledFields;
	
	std::copy(m_FieldOrder.begin(), m_FieldOrder.end(), std::inserter(orderedEnabledFields, orderedEnabledFields.begin()));
	
	
	settings->SetSetting<std::vector<int>>(settingName, orderedEnabledFields);
	
}