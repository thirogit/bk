#include "stdafx.h"
#include "RectObject.h"

namespace bkprint
{

	RectObject::RectObject() : m_brush(NULL), m_pen(NULL), m_Position(0,0,0,0)
	{
	}

	void RectObject::Draw(CDC* pDC)
	{
		CGdiObject* oldBrush;
		CGdiObject* oldPen;

		
		if (m_brush)
			
			oldBrush = pDC->SelectObject(CBrush::FromHandle(m_brush));
		else
		{
			oldBrush = pDC->SelectStockObject(NULL_BRUSH);
		}

		if (m_pen)
			oldPen = pDC->SelectObject(CPen::FromHandle(m_pen));
		else
			oldPen = pDC->SelectStockObject(NULL_PEN);

		CRect rect = m_Position;
		/*switch (m_nShape)
		{
		case rectangle:*/
			pDC->Rectangle(rect);
		/*	break;

		case roundRectangle:
			pDC->RoundRect(rect, m_roundness);
			break;

		case ellipse:
			pDC->Ellipse(rect);
			break;

		case line:
			if (rect.top > rect.bottom)
			{
				rect.top -= m_logpen.lopnWidth.y / 2;
				rect.bottom += (m_logpen.lopnWidth.y + 1) / 2;
			}
			else
			{
				rect.top += (m_logpen.lopnWidth.y + 1) / 2;
				rect.bottom -= m_logpen.lopnWidth.y / 2;
			}

			if (rect.left > rect.right)
			{
				rect.left -= m_logpen.lopnWidth.x / 2;
				rect.right += (m_logpen.lopnWidth.x + 1) / 2;
			}
			else
			{
				rect.left += (m_logpen.lopnWidth.x + 1) / 2;
				rect.right -= m_logpen.lopnWidth.x / 2;
			}

			pDC->MoveTo(rect.TopLeft());
			pDC->LineTo(rect.BottomRight());
			break;
		}*/

		pDC->SelectObject(oldBrush);
		pDC->SelectObject(oldPen);
	}
}