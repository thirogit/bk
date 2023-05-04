#ifndef __PAGE_H__
#define __PAGE_H__

#include <afxwin.h>
#include "Object.h"
#include <vector>

namespace bkprint
{
	class TemplateRenderer;
	class PrintOut;

	class Page
	{
	public:
		void Draw(CDC* pDC);
	private:
		std::vector<Object*> m_objects;

		friend class TemplateRenderer;
		friend class PrintOut;
	};
}

#endif