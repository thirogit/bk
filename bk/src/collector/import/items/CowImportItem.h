#pragma once

#include "ImportItem.h"
#include "../../data/DownloadCow.h"
#include <boost/range/any_range.hpp>



class CowImportItemVisitorFacade : public ImportItemVisitorFacade
{
public:
	virtual DownloadCow* GetCow() = 0;
};


class CowImportItem : public ImportItem
{
	class CowImportItemVisitorFacadeImpl : public ImportItemVisitorFacadeImpl<CowImportItemVisitorFacade>
	{
	public:
		CowImportItemVisitorFacadeImpl(CowImportItem* parent);
		virtual DownloadCow* GetCow();
	private:
		CowImportItem* m_parent;
	};

public:
	CowImportItem(ItemId id);	
	CowImportItem(const CowImportItem& src);
	virtual void Accept(ImportItemVisitor* visitor);	
	DownloadCow GetCow();
private:	
	DownloadCow m_cow;
	
	
};

typedef boost::any_range<CowImportItem*, boost::bidirectional_traversal_tag, CowImportItem*, std::ptrdiff_t> CowImportItemRange;
