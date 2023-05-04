#include "stdafx.h"
#include "Field.h"

namespace bkprint
{
	Field::Field()
	{
	}

	Field::~Field()
	{
	}

	void Field::Unmarshal(pugi::xml_node& node)
	{
		pugi::xml_attribute labelattribute = node.attribute(L"label");
		if (labelattribute)
		{
			m_label = labelattribute.as_string();
		}

		pugi::xml_node positionnode = node.child(L"position");

		pugi::xml_node position_left_node = positionnode.child(L"left");
		pugi::xml_node position_top_node = positionnode.child(L"top");


		pugi::xml_node sizenode = node.child(L"size");

		pugi::xml_node size_width_node = sizenode.child(L"width");
		pugi::xml_node size_height_node = sizenode.child(L"height");

		m_position.x = position_left_node.text().as_int();
		m_position.y = position_top_node.text().as_int();
	

		m_size.SetSize(size_width_node.text().as_int(), size_height_node.text().as_int());



	}

	void Field::CopyFrom(const Field& src)
	{
		m_position = src.m_position;
		m_size = src.m_size;
		m_label = src.m_label;
	}


	const std::wstring& Field::GetLabel() const
	{
		return m_label;
	}

	const CPoint& Field::GetPosition() const
	{
		return m_position;
	}

	const CSize& Field::GetSize() const
	{
		return m_size;
	}

}