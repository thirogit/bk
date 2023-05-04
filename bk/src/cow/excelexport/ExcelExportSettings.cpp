#include "stdafx.h"
#include "ExcelExportSettings.h"

ExcelExportSettings::ExcelExportSettings() : m_bPutDocsInSeperateSheet(false)
{
}

ExcelExportSettings::ExcelExportSettings(const ExcelExportSettings& src)
{
	operator=(src);
}

ExcelExportSettings& ExcelExportSettings::operator=(const ExcelExportSettings& right)
{
	m_bPutDocsInSeperateSheet = right.m_bPutDocsInSeperateSheet;
	m_FieldOrder = right.m_FieldOrder;
	return *this;
}

bool ExcelExportSettings::GetPutDocsInSeperateSheet() const
{
	return m_bPutDocsInSeperateSheet;
}

void ExcelExportSettings::SetPutDocsInSeperateSheet(bool bPutDocsInSeperateSheet)
{
	m_bPutDocsInSeperateSheet = bPutDocsInSeperateSheet;
}

int ExcelExportSettings::GetFieldCount() const
{
	return m_FieldOrder.size();
}

COWDATA_MEMBERS ExcelExportSettings::GetField(int nIndex) const
{
	return m_FieldOrder[nIndex];
}

void ExcelExportSettings::AddField(COWDATA_MEMBERS fieldId)
{
	m_FieldOrder.push_back(fieldId);
}

void ExcelExportSettings::RemoveField(int nIndex)
{
	m_FieldOrder.erase(m_FieldOrder.begin() + nIndex);
}

void ExcelExportSettings::RemoveAllFields()
{
	m_FieldOrder.clear();
}
		
