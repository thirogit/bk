#include "stdafx.h"
#include "FieldsContainer.h"
#include <arrays/DeepDelete.h>
#include "FieldFactory.h"
#include <string/TextUtils.h>
#include "unmarshaler\TemplateUnmarshalException.h"

namespace bkprint
{
	
	FieldsContainer::~FieldsContainer()
	{
		Clear();
	}

	void FieldsContainer::Clear()
	{
		DeepDelete(m_Fields);
		m_Fields.clear();
	}

	void FieldsContainer::AddField(Field* field)
	{
		m_Fields.push_back(field);
	}

	size_t FieldsContainer::Count()
	{
		return m_Fields.size();
	}

	Field* FieldsContainer::GetAt(size_t index)
	{
		return m_Fields[index];
	}
	
	PtrEnumerator<Field> FieldsContainer::EnumFields() const
	{
		return PtrEnumerator<Field>(m_Fields);
	}

	void FieldsContainer::Unmarshal(pugi::xml_node & node)
	{
		auto fieldIt = node.begin();
		FieldFactory fieldsFactory;
		Field* field;
		while (fieldIt != node.end())
		{
			field = fieldsFactory.Create(fieldIt->name());
			if (!field)
				BOOST_THROW_EXCEPTION(TemplateUnmarshalException(TextUtils::Format(L"unsupported field %s", fieldIt->name())));

			field->Unmarshal(*fieldIt);
			m_Fields.push_back(field);
			fieldIt++;
		}
	}

}