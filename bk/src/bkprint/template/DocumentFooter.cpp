#include "stdafx.h"
#include "DocumentFooter.h"
#include <utils\BoolUtil.h>

namespace bkprint
{
	
	DocumentFooter::DocumentFooter() : m_stickToBottom(false)
	{
	}
		
	DocumentFooter::~DocumentFooter()
	{
	}

	bool DocumentFooter::GetStickToBottom() const
	{
		return m_stickToBottom;
	}
	
	void DocumentFooter::Unmarshal(pugi::xml_node & node)
	{
		FieldsContainer::Unmarshal(node);

		pugi::xml_attribute stickToBottomNode = node.attribute(L"stickToBottom");
		m_stickToBottom = BoolUtil::FromString(stickToBottomNode.as_string());
	}

}