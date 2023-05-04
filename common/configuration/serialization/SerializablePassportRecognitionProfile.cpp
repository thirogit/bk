#include "stdafx.h"
#include "SerializablePassportRecognitionProfile.h"
#include "../passportrecognition/PassportFields.h"
#include "../../CommonTypes.h"

SerializablePassportRecognitionProfile::SerializablePassportRecognitionProfile(PassportRecognitionProfile* pProfile) : m_pProfile(pProfile)
{
}

void SerializablePassportRecognitionProfile::serialize(SF::Archive &archive)
{
	PassportFields allPassportFields;
	std::vector<PassportFieldDescriptor*> fieldsToSerialize;
	IPassportField* field;
	PassportFieldDescriptor* fieldDesc;
	PassportFieldMetrics metrics;

	PtrEnumerator<PassportFieldDescriptor> fieldEnum = allPassportFields.EnumFields();
	while (fieldEnum.hasNext())
	{
		fieldsToSerialize.push_back(*fieldEnum);
		fieldEnum.advance();
	}

	count_t fieldCount = fieldsToSerialize.size();
	
	archive & Id();
	archive & ProfileName();
	archive & XDimention();
	archive & YDimention();
	archive & fieldCount;

	if (archive.isWrite())
	{
		for (count_t i = 0; i < fieldCount; i++)
		{
			fieldDesc = fieldsToSerialize[i];
			field = m_pProfile->GetFieldByName(fieldDesc->GetFieldName());
			metrics = field->GetFieldMetrics();

			archive & fieldDesc->GetFieldName();
			archive & metrics.GetX();
			archive & metrics.GetY();
			archive & metrics.GetW();
			archive & metrics.GetH();			
		}
	}
	else
	{
		int x, y, w, h;
		std::wstring fieldName;

		for (count_t i = 0; i < fieldCount; i++)
		{			
			archive & fieldName;
			archive & x;
			archive & y;
			archive & w;
			archive & h;
			m_pProfile->SetFieldMetrics(fieldName, PassportFieldMetrics(x, y, w, h));
		}
		
	}
}

uint32_t& SerializablePassportRecognitionProfile::Id()
{
	return m_pProfile->m_Id;
}

std::wstring& SerializablePassportRecognitionProfile::ProfileName()
{
	return m_pProfile->m_sProfileName;
}

int& SerializablePassportRecognitionProfile::XDimention()
{
	return m_pProfile->m_iDimX;
}

int& SerializablePassportRecognitionProfile::YDimention()
{
	return m_pProfile->m_iDimY;
}

