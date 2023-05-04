#include "stdafx.h"
#include "InDocImportItem.h"
#include "ImportItemVisitor.h"
#include <boost\foreach.hpp>


InDocImportItem::InDocImportItemVisitorFacadeImpl::InDocImportItemVisitorFacadeImpl(InDocImportItem* parent) : m_parent(parent), ImportItemVisitorFacadeImpl<InDocImportItemVisitorFacade>(parent)
{
}

DownloadDoc* InDocImportItem::InDocImportItemVisitorFacadeImpl::GetInDoc()
{
	return &m_parent->m_indoc;
}

CowImportItemRange InDocImportItem::InDocImportItemVisitorFacadeImpl::Cows()
{
	return CowImportItemRange(m_parent->m_cows);
}
	
//#################################################################################################

InDocImportItem::InDocImportItem(ItemId id) : ImportItem(id)
{
}

InDocImportItem::InDocImportItem(const InDocImportItem& src) : ImportItem(src.GetId())
{
	m_indoc = src.m_indoc;
	BOOST_FOREACH(ImportProblem* problem, src.m_problems)
	{
		m_problems.push_back(problem);
	}
}

void InDocImportItem::Accept(ImportItemVisitor* visitor)
{
	InDocImportItemVisitorFacadeImpl facade(this);
	visitor->Visit(&facade);
}

DownloadDoc& InDocImportItem::GetInDoc()
{
	return m_indoc;
}





