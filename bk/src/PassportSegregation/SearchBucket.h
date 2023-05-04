#ifndef __SEARCHBUCKET_H__
#define __SEARCHBUCKET_H__

#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include "BucketEntry.h"



class SearchBucket
{
	struct index_by_cowNo{};

	struct BucketEntryKey
	{
	  typedef CowNo result_type;
	  result_type operator()(BucketEntry* pEntry) const 
	  {
		  return pEntry->GetPassport().GetCowNo();
	  }
	};

	typedef boost::multi_index_container
	<
	  BucketEntry*,
	  boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<index_by_cowNo>, BucketEntryKey >										
									>
	> BucketIndex;

public:
	SearchBucket();
	~SearchBucket();

	size_t size() const;
	BucketEntry* find(const CowNo& cowNo) const;
	void add(const CowPassport& passport);
	BucketEntry* at(int index) const;

private:
	BucketIndex m_entries;
};

#endif