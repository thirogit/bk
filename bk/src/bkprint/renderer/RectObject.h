#ifndef __RECTOBJECT_H__
#define __RECTOBJECT_H__

#include "Object.h"
#include <afxwin.h>

namespace bkprint
{
	class TemplateRenderer;

	class RectObject : public Object
	{
	public:
		RectObject();
		virtual void Draw(CDC* pDC);
	protected:	
		HBRUSH m_brush;
		HPEN m_pen;
		CRect m_Position;
		
		friend class TemplateRenderer;
	};

}

#endif 
