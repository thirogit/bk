#ifndef __PRINTOUT_H__
#define __PRINTOUT_H__

#include "Page.h"
#include <vector>
#include "Paper.h"

namespace bkprint
{
	class TemplateRenderer;

	class PrintOut
	{
	public:
		PrintOut();
		~PrintOut();
		//PrintOut(const PrintOut& src);
		PrintOut(PrintOut&& src);

		void DrawPage(CDC* pDC, int page);
		int GetNumberOfPages();
		const Paper& GetPaper() const;
		PrintOut& operator=(PrintOut&& src);

	private:
		void MoveFom(PrintOut& src);
		void Destroy();

	private:
		std::vector<Page*> m_pages;
		Paper m_paper;

		std::vector<HGDIOBJ> m_usedObjects;
		
		friend class TemplateRenderer;
	};
}

#endif