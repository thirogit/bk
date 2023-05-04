#include "stdafx.h"
#include "OutDocImportItem.h"
#include "ImportItemVisitor.h"
#include <boost\foreach.hpp>

OutDocImportItem::OutDocImportItemVisitorFacadeImpl::OutDocImportItemVisitorFacadeImpl(OutDocImportItem* parent) : ImportItemVisitorFacadeImpl<OutDocImportItemVisitorFacade>(parent), m_parent(parent)
{
}

DownloadDoc* OutDocImportItem::OutDocImportItemVisitorFacadeImpl::GetOutDoc()
{
	return &m_parent->m_outdoc;
}


CowImportItemRange OutDocImportItem::OutDocImportItemVisitorFacadeImpl::Cows()
{
	return m_parent->Cows();
}

//#################################################################################################

OutDocImportItem::OutDocImportItem(ItemId id) : ImportItem(id)
{
}

OutDocImportItem::OutDocImportItem(const OutDocImportItem& src) : ImportItem(src.GetId())
{
	m_outdoc = src.m_outdoc;

	BOOST_FOREACH(ImportProblem* problem, src.m_problems)
	{
		m_problems.add_item(problem);
	}
}

void OutDocImportItem::Accept(ImportItemVisitor* visitor)
{
	OutDocImportItemVisitorFacadeImpl facade(this);
	visitor->Visit(&facade);
}

DownloadDoc OutDocImportItem::GetOutDoc()
{

	return m_outdoc;
}


CowImportItemRange OutDocImportItem::Cows()
{
	return CowImportItemRange(m_cows);
}



