#pragma once

#include "../../data/ICowEntry.h"
#include <vector>
#include "../../data/IDoc.h"
#include <cstdint>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include "ChangeEvent.h"

class DocChange
{
public:
	DocChange(ChangeEvent event,ICowEntry* pOld,ICowEntry* pNew);
	DocChange(const DocChange& src);

	ICowEntry* Old() const;
	ICowEntry* New() const;
	ChangeEvent Event() const;	
private:
	ICowEntry* m_pOld;
	ICowEntry* m_pNew;
	ChangeEvent  m_event;	
};

class DocChanges
{
public:
	typedef std::vector<DocChange*> change_vector;
	typedef change_vector::const_iterator const_iterator;
	typedef change_vector::iterator iterator;
	
	DocChanges();
	~DocChanges();
	DocChanges(DocChanges&& changes);
	DocChanges(const DocChanges& changes);

	void add(ChangeEvent event,ICowEntry* pOld,ICowEntry* pNew);	

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();
	
private:
	change_vector m_changes;
	
};

class DocChangeDetector
{
	struct index_tag_cowid{};

	typedef boost::multi_index_container
	<
		ICowEntry*,
		boost::multi_index::indexed_by<	boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_cowid>, boost::multi_index::const_mem_fun<ICowEntry,uint32_t, &ICowEntry::GetCowId> > >
	> DocEntriesIndex;

public:
	DocChangeDetector(IDoc* pOldDoc,IDoc* pNewDoc);
	DocChanges Detect();
private:
	IDoc* m_pOldDoc;
	IDoc* m_pNewDoc;
};