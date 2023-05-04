#pragma once

#include "ImportItem.h"
#include "../../data/DownloadDoc.h"
#include "../problems/ImportProblem.h"
#include "CowImportItem.h"

class InDocImportItemVisitorFacade : public ImportItemVisitorFacade
{
public:
	virtual DownloadDoc* GetInDoc() = 0;
	virtual CowImportItemRange Cows() = 0;
};

class InDocImportItem : public ImportItem
{
	class InDocImportItemVisitorFacadeImpl : public ImportItemVisitorFacadeImpl<InDocImportItemVisitorFacade>
	{
	public:
		InDocImportItemVisitorFacadeImpl(InDocImportItem* parent);
		virtual DownloadDoc* GetInDoc();
		virtual CowImportItemRange Cows();
	private:
		InDocImportItem* m_parent;
	};

public:
	InDocImportItem(ItemId id);
	InDocImportItem(const InDocImportItem& src);
	virtual void Accept(ImportItemVisitor* visitor);

	DownloadDoc& GetInDoc();
	CowImportItemRange Cows();

private:	
	DownloadDoc m_indoc;
	std::vector<ImportProblem*> m_problems;
	std::vector<CowImportItem*> m_cows;
	
	friend class ImportItemsFactory;
};

typedef boost::any_range<InDocImportItem*, boost::bidirectional_traversal_tag, InDocImportItem*, std::ptrdiff_t> InDocImportItemRange;
