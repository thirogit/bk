#pragma once

#include "Field.h"
#include <arrays/Enumerator.h>
#include <vector>

namespace bkprint
{
	class FieldsContainer
	{
	public:
		~FieldsContainer();
		void Unmarshal(pugi::xml_node & node);
		void AddField(Field* field);
		size_t Count();
		Field* GetAt(size_t index);
		PtrEnumerator<Field> EnumFields() const;
		void Clear();
	private:
		std::vector<Field*> m_Fields;
	};
}
