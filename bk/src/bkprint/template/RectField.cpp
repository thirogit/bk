#include "stdafx.h"
#include "RectField.h"
#include "util\ColorUtil.h"
#include "unmarshaler\TypesUnmarshaler.h"

namespace bkprint
{

	RectField::RectField()
	{
	}

	RectField::~RectField()
	{

	}

	Field* RectField::Clone()
	{
		RectField* clone = new RectField();
		clone->CopyFrom(*this);
		return clone;
	}

	void RectField::CopyFrom(const RectField& src)
	{
		Field::CopyFrom(src);
		m_border = src.m_border;
		m_fillClr = src.m_fillClr;
	}


	void RectField::Unmarshal(pugi::xml_node & node)
	{
		Field::Unmarshal(node);

		TypesUnmarshaler unmarshaller;

		pugi::xml_node bordernnode = node.child(L"border");
		if (bordernnode)
		{			
			this->m_border = unmarshaller.UnmarshalBorder(bordernnode);
		}
		else
		{
			this->m_border.MakeNull();
		}


		pugi::xml_node fillColor = node.child(L"fillColor");
		m_fillClr = unmarshaller.UnmarshalColor(fillColor);
		
	}

	void RectField::Visit(FieldVisitor* visitor)
	{
		visitor->Accept(this);
	}

	const Border& RectField::GetBorder() const
	{
		return m_border;
	}

	void RectField::SetBorder(const Border& border)
	{
		m_border = border;
	}

	const Color& RectField::GetFillColor() const
	{
		return m_fillClr;
	}

	void RectField::SetFillColor(const Color& color)
	{
		m_fillClr = color;
	}
}
