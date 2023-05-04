#include "stdafx.h"
#include "Paper.h"

namespace bkprint
{

	Paper::Paper() : m_type(PaperType_None), m_orientation(PaperOrientation_None)
	{
	}

	Paper::Paper(PaperType type, PaperOrientation orientation) : m_type(type), m_orientation(orientation)
	{
	}

	Paper::Paper(const Paper& src)
	{
		operator=(src);
	}

	Paper& Paper::operator=(const Paper& right)
	{
		m_type = right.m_type;
		m_orientation = right.m_orientation;
		return *this;
	}

	PaperType Paper::GetType() const
	{
		return m_type;
	}

	PaperOrientation Paper::GetOrientation() const
	{
		return m_orientation;
	}

	void Paper::SetType(PaperType type)
	{
		m_type = type;
	}

	void Paper::SetOrientation(PaperOrientation orientation)
	{
		m_orientation = orientation;
	}

}