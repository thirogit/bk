#ifndef __COLOREDOBJECT_H__
#define __COLOREDOBJECT_H__

#include "../uitypes/Color.h"

template<class O>
class ColoredObject
{
public:
	ColoredObject()
	{
	}

	ColoredObject(const O& object) : m_object(object)
	{
	}

	ColoredObject(const O& object,const Color& color) : m_object(object),m_color(color)
	{
	}
	
	ColoredObject(const ColoredObject<O>& src) : m_object(src.m_object),m_color(src.m_color)
	{
	}

	ColoredObject<O>& operator=(const ColoredObject<O>& right)
	{
		m_object = right.m_object;
		m_color = right.m_color;
		return *this;
	}

	const Color& GetColor() const
	{
		return m_color;
	}
	
	const O& Object() const
	{
		return m_object;
	}
	
private:
	O m_object;
	Color m_color;
};


#endif