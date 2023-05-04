#ifndef __COMPOSITEOBJECT_H__
#define __COMPOSITEOBJECT_H__

#include "Object.h"
#include <afxwin.h>
#include <vector>

class TemplateRenderer;

namespace bkprint
{
	class CompositeObject : public Object
	{
	public:
		CompositeObject();
		~CompositeObject();
		
		virtual void Draw(CDC* pDC);

		void SetPosition(const CRect& position);
	protected:			
		CRect m_position;
		std::vector<Object*> m_objects;
		
		friend class TemplateRenderer;
	};

}

#endif 
