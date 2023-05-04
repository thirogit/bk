#include "stdafx.h"
#include "Page.h"
#include <boost\foreach.hpp>

namespace bkprint
{

	void Page::Draw(CDC* pDC)
	{
		BOOST_FOREACH(Object* obj, m_objects)
		{
			obj->Draw(pDC);
		}		
	}
}