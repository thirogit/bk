#include "stdafx.h"
#include "PassportFields.h"


PassportFieldDescriptor PassportFields::BIRTHINFO_FIELD(L"BIRTHINFO", false);
PassportFieldDescriptor PassportFields::EAN_FIELD(L"EAN", false);
PassportFieldDescriptor PassportFields::EANBARCODE_FIELD(L"EANBARCODE", true);
PassportFieldDescriptor PassportFields::FARMNOBARCODE_FIELD(L"FARMNOBARCODE", true);
PassportFieldDescriptor PassportFields::FIRSTOWNER_FIELD(L"FIRSTOWNER", false);
PassportFieldDescriptor PassportFields::MOTHEREAN_FIELD(L"MOTHEREAN", false);
PassportFieldDescriptor PassportFields::PASSDATE_FIELD(L"PASSDATE", false);
PassportFieldDescriptor PassportFields::PASSNO_FIELD(L"PASSNO", false);
PassportFieldDescriptor PassportFields::SEX_FIELD(L"SEX", false);
PassportFieldDescriptor PassportFields::STOCK_FIELD(L"STOCK", false);

PassportFields::PassportFields()
{
/*	m_fields = {	&BIRTHINFO_FIELD, 
					&EAN_FIELD, 
					&EANBARCODE_FIELD,
					&FARMNOBARCODE_FIELD,
					&FIRSTOWNER_FIELD,
					&MOTHEREAN_FIELD,
					&PASSDATE_FIELD,
					&PASSNO_FIELD,
					&SEX_FIELD,
					&STOCK_FIELD
	};*/
	
}


PtrEnumerator<PassportFieldDescriptor> PassportFields::EnumFields()
{
	return PtrEnumerator<PassportFieldDescriptor>(m_fields);
}

PassportFieldDescriptor* PassportFields::GetField(const std::wstring& fieldName)
{
	const boost::multi_index::index<FieldsIndex, index_tag_fieldname>::type& indexByName = m_fields.get<index_tag_fieldname>();

	auto fieldIt = indexByName.find(fieldName);

	if (fieldIt != indexByName.end())
		return *fieldIt;

	return NULL;

}
