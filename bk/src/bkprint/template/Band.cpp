#include "stdafx.h"
#include "Band.h"

namespace bkprint
{
	Band::Band(const std::wstring& name) : m_name(name)
	{

	}

	void Band::Unmarshal(pugi::xml_node & node)
	{
		pugi::xml_node headernode = node.child(L"header");
		if (headernode)
		{
			m_header.Unmarshal(headernode);
		}
		pugi::xml_node fieldsnode = node.child(L"fields");

		if (fieldsnode)
		{
			m_body.Unmarshal(fieldsnode);
		}

		pugi::xml_node footernode = node.child(L"footer");
		if (footernode)
		{
			m_footer.Unmarshal(footernode);
		}		
	}

	FieldsContainer& Band::FooterFields()
	{
		return m_footer;
	}

	FieldsContainer& Band::BodyFields()
	{
		return m_body;
	}

	FieldsContainer& Band::HeaderFields()
	{
		return m_header;
	}

	const std::wstring& Band::Name() const
	{
		return m_name;
	}
	
}