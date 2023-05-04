#include "stdafx.h"
#include "PageHeader.h"
#include <utils\BoolUtil.h>

namespace bkprint
{
	
	PageHeader::PageHeader() : m_showOnFirstPage(false), m_topMargin(0)
	{
	}
		
	PageHeader::~PageHeader()
	{
	}

	bool PageHeader::GetShowOnFirstPage() const
	{
		return m_showOnFirstPage;
	}

	int PageHeader::GetTopMargin() const
	{
		return max(0, m_topMargin);
	}
	
	void PageHeader::Unmarshal(pugi::xml_node & node)
	{
		FieldsContainer::Unmarshal(node);

		pugi::xml_attribute showOnFirstPageNode = node.attribute(L"showOnFirstPage");
		m_showOnFirstPage = BoolUtil::FromString(showOnFirstPageNode.as_string());


		pugi::xml_attribute topMarginNode = node.attribute(L"topMargin");
		m_topMargin = topMarginNode.as_int();		
	}

}