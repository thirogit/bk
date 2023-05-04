#include "stdafx.h"
#include "ImgObject.h"

namespace bkprint
{
	ImgObject::ImgObject() 
	{

	}
	

	ImgObject::~ImgObject()
	{
		
	}

	void ImgObject::Draw(CDC* pDC)
	{
		RectObject::Draw(pDC);
		if (m_image.isValid() && m_Position.Height() > 0 && m_Position.Width() > 0)
		{
			m_image.draw(pDC->GetSafeHdc(), m_Position);
		}
	}
}