#include "stdafx.h"
#include "PrintOut.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include <algorithm>

namespace bkprint
{

	PrintOut::PrintOut()
	{
	}

	PrintOut::~PrintOut()
	{		
		Destroy();
	}

	void PrintOut::Destroy()
	{
		BOOST_FOREACH(Page* page, m_pages)
		{
			DeepDelete(page->m_objects);
		}

		DeepDelete(m_pages);
		m_pages.clear();

		BOOST_FOREACH(HGDIOBJ object, m_usedObjects)
		{
			DeleteObject(object);
		}
		m_usedObjects.clear();
		
	}

	void PrintOut::MoveFom(PrintOut& src)
	{
		std::copy(src.m_pages.begin(), src.m_pages.end(), std::back_inserter(m_pages));
		src.m_pages.clear();

		std::copy(src.m_usedObjects.begin(), src.m_usedObjects.end(), std::back_inserter(m_usedObjects));
		src.m_usedObjects.clear();

		m_paper = src.m_paper;
		src.m_paper = Paper(PaperType_None, PaperOrientation_None);
	}

	PrintOut& PrintOut::operator=(PrintOut&& src)
	{
		Destroy();
		MoveFom(src);
		return *this;
	}

	/*PrintOut::PrintOut(const PrintOut& src)
	{
		BOOST_FOREACH(Page* page, src.m_pages)
		{
			m_pages.push_back(page->Clone());
		}

		m_paper = src.m_paper;
	}
*/
	PrintOut::PrintOut(PrintOut&& src)
	{
		MoveFom(src);
	}

	const Paper& PrintOut::GetPaper() const
	{
		return m_paper;
	}

	void PrintOut::DrawPage(CDC* pDC, int nPage)
	{
		if (nPage >= 0 && nPage < m_pages.size())
		{
			Page* page = m_pages[nPage];
			page->Draw(pDC);
		}
	}

	int PrintOut::GetNumberOfPages()
	{
		return m_pages.size();
	}
}