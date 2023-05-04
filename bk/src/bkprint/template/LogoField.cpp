#include "stdafx.h"
#include "LogoField.h"
#include "util\ColorUtil.h"
#include "unmarshaler\TypesUnmarshaler.h"

namespace bkprint
{

	LogoField::LogoField()
	{
	}

	LogoField::~LogoField()
	{

	}

	Field* LogoField::Clone()
	{
		LogoField* clone = new LogoField();
		clone->CopyFrom(*this);
		return clone;
	}

	void LogoField::CopyFrom(const LogoField& src)
	{
		RectField::CopyFrom(src);
		m_image = src.m_image;
	}


	void LogoField::Unmarshal(pugi::xml_node & node)
	{
		RectField::Unmarshal(node);
	}

	void LogoField::Visit(FieldVisitor* visitor)
	{
		visitor->Accept(this);
	}

	const fipWinImage& LogoField::GetImage() const
	{
		return m_image;
	}

	void LogoField::SetImage(const fipImage& src)
	{
		m_image = src;
	}
}
