#include "stdafx.h"
#include "PassportRecognitionProfile.h"
#include <string>
#include <arrays/DeepDelete.h>
#include "PassportFields.h"

PassportRecognitionProfile::PassportRecognitionProfile(uint32_t profileId)
{
	m_iDimX = 0;
	m_iDimY = 0;
	m_Id = profileId;

	PassportFieldDescriptor* fieldDesc;
	PassportFields fields;
	PtrEnumerator<PassportFieldDescriptor> fieldEnum = fields.EnumFields();
	while (fieldEnum.hasNext())
	{
		 fieldDesc = *fieldEnum;
		 m_FieldsIndex.insert(new PassportField(fieldDesc));
		 fieldEnum.advance();
	}
}


PassportRecognitionProfile::PassportRecognitionProfile(const IPassportRecognitionProfile* src) : m_Id(src->GetId())
{
	CopyFrom(src);
}

PassportRecognitionProfile::PassportRecognitionProfile(const PassportRecognitionProfile& copy)
{
	CopyFrom(&copy);
}

PassportRecognitionProfile::~PassportRecognitionProfile()
{
	DeepDelete(m_FieldsIndex);
}

void PassportRecognitionProfile::operator=(const PassportRecognitionProfile& copy)
{
	CopyFrom(&copy);
}

void PassportRecognitionProfile::CopyFrom(const IPassportRecognitionProfile* src)
{
	m_iDimX = src->GetXDimention();
	m_iDimY = src->GetYDimention();
	m_sProfileName = src->GetProfileName();

	CopyFieldsFrom(src);
}

int PassportRecognitionProfile::GetXDimention() const
{
	return m_iDimX;
}

int PassportRecognitionProfile::GetYDimention() const
{
	return m_iDimY;	
}

void PassportRecognitionProfile::SetXDimention(int dimx)
{
	m_iDimX = dimx;
}

void PassportRecognitionProfile::SetYDimention(int dimy)
{
	m_iDimY = dimy;
}


uint32_t PassportRecognitionProfile::GetId() const
{
	return m_Id;
}

const std::wstring& PassportRecognitionProfile::GetProfileName() const
{
	return m_sProfileName;
}

void PassportRecognitionProfile::SetProfileName(const std::wstring& profileName)
{
	m_sProfileName = profileName;
}

IPassportField* PassportRecognitionProfile::GetFieldByName(const std::wstring& sFieldName) const
{
	return FindFieldByName(sFieldName);
}

PassportField* PassportRecognitionProfile::FindFieldByName(const std::wstring& sFieldName) const
{
	const boost::multi_index::index<FieldsIndex, index_tag_fieldname>::type& indexByName = m_FieldsIndex.get<index_tag_fieldname>();

	auto fieldIt = indexByName.find(sFieldName);

	if (fieldIt != indexByName.end())
		return *fieldIt;

	return NULL;
}

void PassportRecognitionProfile::SetFieldMetrics(const std::wstring& sFieldName, const PassportFieldMetrics& metrics)
{
	PassportField* field = FindFieldByName(sFieldName);
	if (field)
	{
		field->SetFieldMetrics(metrics);
	}
}

void PassportRecognitionProfile::SetFieldMetrics(IPassportField* field)
{
	SetFieldMetrics(field->GetDescriptor()->GetFieldName(), field->GetFieldMetrics());
}

void PassportRecognitionProfile::SetFieldMetric(const std::wstring& sFieldName, FIELD_METRIC metric, int metricValue)
{
	PassportField* field = FindFieldByName(sFieldName);
	if (field)
	{
		field->SetMetric(metric, metricValue);
	}
}
        
void PassportRecognitionProfile::CopyFieldsFrom(const IPassportRecognitionProfile* src)
{
	DeepDelete(m_FieldsIndex);
	m_FieldsIndex.clear();

	IPassportField* field;

	PtrEnumerator<IPassportField> enumFields = src->EnumFields();
	while (enumFields.hasNext())
	{
		field = *enumFields;
		m_FieldsIndex.insert(new PassportField(field));
		enumFields.advance();
	}
}      

PtrEnumerator<IPassportField>  PassportRecognitionProfile::EnumFields() const
{
	return PtrEnumerator<IPassportField>(m_FieldsIndex);
}
