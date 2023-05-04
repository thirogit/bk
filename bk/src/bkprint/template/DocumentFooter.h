#pragma once

#include "FieldsContainer.h"

namespace bkprint
{
	class DocumentFooter : public FieldsContainer
	{
	public:
		DocumentFooter();
		~DocumentFooter();
		virtual void Unmarshal(pugi::xml_node & node);
		bool GetStickToBottom() const;
	private:
		bool m_stickToBottom;
	};
}
