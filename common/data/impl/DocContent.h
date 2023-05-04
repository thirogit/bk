#ifndef __DOCCONTENT_H__
#define __DOCCONTENT_H__

#include "CowEntry.h"
#include "../../arrays/Enumerator.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>

class SerializableDocContent;

class DocContent
{
	struct index_by_id{};

	typedef boost::multi_index_container
	<
	  CowEntry*,
	  boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<index_by_id>, boost::multi_index::const_mem_fun<CowEntry,uint32_t, &CowEntry::GetCowId> >										
									>
	> ContentIndex;


public:
	DocContent();
	~DocContent();
		
	int GetCowCount() const;
	PtrEnumerator<ICowEntry> EnumCows() const;
	PtrEnumerator<CowEntry> EnumCows();
	CowEntry*	GetCow(uint32_t cowId) const;
	CowEntry* GetCowAt(int index) const;
	void RemoveAt(int index);
	void RemoveAll();
	CowEntry* AddCow(uint32_t cowId);
	void AddCow(CowEntry* cowEntry);
	void RemoveCow(uint32_t cowId);
protected:
	ContentIndex m_Cows;

	friend class SerializableDocContent;

};

#endif