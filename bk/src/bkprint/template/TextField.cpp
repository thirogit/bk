#include "stdafx.h"
#include "TextField.h"
#include "unmarshaler\TypesUnmarshaler.h"

namespace bkprint
{
	TextField::TextField()
	{
	}


	TextField::~TextField()
	{
		int a = 0;
		a++;
	}

	Field* TextField::Clone()
	{
		TextField* clone = new TextField();
		clone->CopyFrom(this);
		return clone;
	}

	void TextField::CopyFrom(const TextField* pSrc)
	{
		m_text = pSrc->m_text;
		m_border = pSrc->m_border;
		m_fillColor = pSrc->m_fillColor;
		m_ownStyle = pSrc->m_ownStyle;
		m_refStyleId = pSrc->m_refStyleId;
	}

	void TextField::SetText(const std::wstring& sText)
	{
		m_text = sText;
	}

	const std::wstring& TextField::GetText() const
	{
		return m_text;
	}

	void TextField::Unmarshal(pugi::xml_node & node)
	{
		Field::Unmarshal(node);
				
		TypesUnmarshaler unmarshaller;
		pugi::xml_node ownstyle = node.child(L"ownstyle");
		if (ownstyle)
		{
			
			m_ownStyle = unmarshaller.UnmarshalTextStyle(ownstyle);
		}
		else
		{
			m_ownStyle.MakeNull();
			pugi::xml_node refstyle = node.child(L"style");
			m_refStyleId = refstyle.text().as_string();
		}

		pugi::xml_node value = node.child(L"value");
		m_text = value.text().as_string();

		pugi::xml_node border = node.child(L"border");
		m_border = unmarshaller.UnmarshalBorder(border);

		pugi::xml_node fillColor = node.child(L"fillColor");
		m_fillColor = unmarshaller.UnmarshalColor(fillColor);
	}

	void TextField::Visit(FieldVisitor* visitor)
	{
		visitor->Accept(this);
	}

	const Border& TextField::GetBorder() const
	{
		return m_border;
	}

	void TextField::SetBorder(const Border& border)
	{
		m_border = border;
	}

	const Color& TextField::GetFillColor() const
	{
		return m_fillColor;
	}

	void TextField::SetFillColor(const Color& color)
	{
		m_fillColor = color;
	}

	const TextStyle& TextField::GetOwnStyle() const
	{
		return m_ownStyle;
	}

	void TextField::SetOwnStyle(const TextStyle& ownstyle)
	{
		m_ownStyle = ownstyle;
	}

	const std::wstring& TextField::GetRefStyle() const
	{
		return m_refStyleId;
	}

	void TextField::SetRefStyle(const std::wstring& refStyleId)
	{
		m_refStyleId = refStyleId;
	}

}

