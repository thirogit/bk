#ifndef __PASSPORTFIELDS_H__
#define __PASSPORTFIELDS_H__

#include "PassportFieldDescriptor.h"
#include <arrays/Enumerator.h>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>


class PassportFields
{
private:

	struct index_tag_fieldname{};

	typedef boost::multi_index_container
		<
		PassportFieldDescriptor*,
		boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_fieldname>, boost::multi_index::const_mem_fun<PassportFieldDescriptor, const std::wstring&, &PassportFieldDescriptor::GetFieldName> >
		>
		> FieldsIndex;

	FieldsIndex m_fields;

public:
	static PassportFieldDescriptor BIRTHINFO_FIELD;
	static PassportFieldDescriptor EAN_FIELD;
	static PassportFieldDescriptor EANBARCODE_FIELD;
	static PassportFieldDescriptor FARMNOBARCODE_FIELD;
	static PassportFieldDescriptor FIRSTOWNER_FIELD;
	static PassportFieldDescriptor MOTHEREAN_FIELD;
	static PassportFieldDescriptor PASSDATE_FIELD;
	static PassportFieldDescriptor PASSNO_FIELD;
	static PassportFieldDescriptor SEX_FIELD;
	static PassportFieldDescriptor STOCK_FIELD;
	
	PassportFields();
	PtrEnumerator<PassportFieldDescriptor> EnumFields();
	PassportFieldDescriptor* GetField(const std::wstring& fieldName);
};


#endif 
