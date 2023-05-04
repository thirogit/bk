#include "stdafx.h"
#include "CompositeObject.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>

namespace bkprint
{
	CompositeObject::CompositeObject()
	{
	}

	CompositeObject::~CompositeObject()
	{
		DeepDelete(m_objects);
	}

	void CompositeObject::SetPosition(const CRect& position)
	{
		m_position = position;
	}

	void CompositeObject::Draw(CDC* pDC)
	{
		int savedState = pDC->SaveDC();

		pDC->SetWindowOrg(-this->m_position.left, -this->m_position.top);
		BOOST_FOREACH(Object* obj, m_objects)
		{
			obj->Draw(pDC);
		}

		pDC->RestoreDC(savedState);

	}

}