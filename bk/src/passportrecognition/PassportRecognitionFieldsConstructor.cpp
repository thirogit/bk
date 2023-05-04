#include "stdafx.h"
#include "PassportRecognitionFieldsConstructor.h"
#include <configuration\passportrecognition\PassportFields.h>
#include <map>

PassportRecognitionFieldsConstructor::PassportRecognitionFieldsConstructor(PassportRecognitionResult* pResult)
{
	m_pResult = pResult;
	

	add_to_map(m_FieldSettersMap).put(&PassportFields::BIRTHINFO_FIELD, &PassportRecognitionResult::SetBirthDatePlace)
		(&PassportFields::BIRTHINFO_FIELD, &PassportRecognitionResult::SetBirthDatePlace)
		(&PassportFields::EAN_FIELD, &PassportRecognitionResult::SetCowNo)
		(&PassportFields::EANBARCODE_FIELD, &PassportRecognitionResult::SetCowNoBC)
		(&PassportFields::FARMNOBARCODE_FIELD, &PassportRecognitionResult::SetFarmNoBC)
		(&PassportFields::FIRSTOWNER_FIELD, &PassportRecognitionResult::SetFstOwner)
		(&PassportFields::MOTHEREAN_FIELD, &PassportRecognitionResult::SetMotherNo)
		(&PassportFields::PASSDATE_FIELD, &PassportRecognitionResult::SetPassDate)
		(&PassportFields::PASSNO_FIELD, &PassportRecognitionResult::SetPassNo)
		(&PassportFields::SEX_FIELD, &PassportRecognitionResult::SetCowSex)
		(&PassportFields::STOCK_FIELD, &PassportRecognitionResult::SetCowStock);

}

void PassportRecognitionFieldsConstructor::SetField(const PassportFieldDescriptor* pField,const std::wstring& sValue)
{
	FieldSettersMap::iterator it = m_FieldSettersMap.find(pField->GetFieldName());
	if(it != m_FieldSettersMap.end())
	{
		(m_pResult->*(it->second))(sValue);
		it++;
	}
}


	
