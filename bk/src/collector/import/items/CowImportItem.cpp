#include "stdafx.h"
#include "CowImportItem.h"
#include "ImportItemVisitor.h"
#include <boost\foreach.hpp>

CowImportItem::CowImportItemVisitorFacadeImpl::CowImportItemVisitorFacadeImpl(CowImportItem* parent) : m_parent(parent), ImportItemVisitorFacadeImpl<CowImportItemVisitorFacade>(parent)
{

}

DownloadCow* CowImportItem::CowImportItemVisitorFacadeImpl::GetCow()
{
	return &m_parent->m_cow;
}


//#################################################################################################

CowImportItem::CowImportItem(ItemId id) : ImportItem(id)
{
}

CowImportItem::CowImportItem(const CowImportItem& src) : ImportItem(src.GetId())
{
	m_cow = src.m_cow;

	BOOST_FOREACH(ImportProblem* problem, src.m_problems)
	{
		m_problems.add_item(problem);
	}
}

void CowImportItem::Accept(ImportItemVisitor* visitor)
{
	CowImportItemVisitorFacadeImpl facade(this);
	visitor->Visit(&facade);
}

DownloadCow CowImportItem::GetCow()
{
	return m_cow;
}



