#pragma once

#include "FieldsContainer.h"

namespace bkprint
{
	class PageHeader : public FieldsContainer
	{
	public:
		PageHeader();
		~PageHeader();
		virtual void Unmarshal(pugi::xml_node & node);
		bool GetShowOnFirstPage() const;
		int GetTopMargin() const;
	private:
		bool m_showOnFirstPage;
		int m_topMargin;
	};
}
