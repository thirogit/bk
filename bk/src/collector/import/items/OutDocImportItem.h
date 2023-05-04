#pragma once

#include "ImportItem.h"
#include "../../data/DownloadDoc.h"
#include "../problems/ImportProblem.h"
#include "CowImportItem.h"


class OutDocImportItemVisitorFacade : public ImportItemVisitorFacade
{
public:
	virtual DownloadDoc* GetOutDoc() = 0;
	virtual CowImportItemRange Cows() = 0;
};

class OutDocImportItem : public ImportItem
{
	class OutDocImportItemVisitorFacadeImpl : public ImportItemVisitorFacadeImpl<OutDocImportItemVisitorFacade>
	{
	public:
		OutDocImportItemVisitorFacadeImpl(OutDocImportItem* parent);
		virtual DownloadDoc* GetOutDoc();
		virtual CowImportItemRange Cows();
	private:
		OutDocImportItem* m_parent;
	};

public:
	OutDocImportItem(ItemId id);
	OutDocImportItem(const OutDocImportItem& src);
	virtual void Accept(ImportItemVisitor* visitor);

	DownloadDoc GetOutDoc();
	CowImportItemRange Cows();
private:
	DownloadDoc m_outdoc;	
	std::vector<CowImportItem*> m_cows;
	
};

typedef boost::any_range<OutDocImportItem*, boost::bidirectional_traversal_tag, OutDocImportItem*, std::ptrdiff_t> OutDocImportItemRange;

