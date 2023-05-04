#pragma once

#include <data/runtime/IItem.h>
#include <boost/range/any_range.hpp>
#include "../problems/ImportProblem.h"
#include <data\runtime\ItemIndex.h>

class ImportItemVisitor;

typedef boost::any_range<ImportProblem*, boost::bidirectional_traversal_tag, ImportProblem*, std::ptrdiff_t> ImportProblemRange;


class ImportItemVisitorFacade
{
public:
	virtual void AddProblem(const ImportProblem* problem) = 0;
	virtual void RemoveProblem(ItemId problemId) = 0;
	virtual ImportProblemRange Problems() = 0;
	virtual ImportProblem* GetProblem(ItemId id) = 0;
};

class ImportItem;

template<class T>
class ImportItemVisitorFacadeImpl : public T
{
public:
	ImportItemVisitorFacadeImpl(ImportItem* parent) : m_parent(parent)
	{
	}
	virtual void AddProblem(const ImportProblem* problem) 
	{
		m_parent->m_problems.add_item(problem->Clone());
	}
	virtual void RemoveProblem(ItemId problemId)
	{
		m_parent->m_problems.remove_item(problemId);
	}
	virtual ImportProblemRange Problems()
	{
		return ImportProblemRange(m_parent->m_problems);
	}
	virtual ImportProblem* GetProblem(ItemId id)
	{
		return m_parent->m_problems.find_item(id);
	}
private:
	ImportItem* m_parent;
};

class ImportItem : public IItem
{
protected:	

public:
	ImportItem(ItemId id);
	virtual ItemId GetId() const;	
	virtual void Accept(ImportItemVisitor* visitor) = 0;
	ImportProblemRange Problems();
	ImportProblem* GetProblem(ItemId id);
protected:	
	ItemIndex<ImportProblem> m_problems;
private:
	ItemId m_id;

	template<class T> friend class ImportItemVisitorFacadeImpl;
};
