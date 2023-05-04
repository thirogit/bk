#ifndef __IMGOBJECT_H__
#define __IMGOBJECT_H__

#include "RectObject.h"
#include <afxwin.h>
#include "FreeImagePlus.h"


namespace bkprint
{
	class TemplateRenderer;

	class ImgObject : public RectObject
	{
	public:
		ImgObject();
		~ImgObject();
		virtual void Draw(CDC* pDC);
	protected:	
		fipWinImage m_image;
		
		friend class TemplateRenderer;
	};

}

#endif 
