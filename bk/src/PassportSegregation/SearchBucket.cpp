#include "stdafx.h"
#include "SearchBucket.h"
#include <arrays\DeepDelete.h>

SearchBucket::SearchBucket()
{

}

SearchBucket::~SearchBucket()
{
	DeepDelete(m_entries);
}

size_t SearchBucket::size() const
{
	return m_entries.size();
}

BucketEntry* SearchBucket::find(const CowNo& cowNo) const
{
	const boost::multi_index::index<BucketIndex,index_by_cowNo>::type& entriesByCowNo = 
		boost::multi_index::get<index_by_cowNo>(m_entries);

	auto it  = entriesByCowNo.find(cowNo);

	if(it != entriesByCowNo.end())
		return *it;

	return NULL;
}

BucketEntry* SearchBucket::at(int index) const
{
	return m_entries.at(index);
}

void SearchBucket::add(const CowPassport& passport)
{
	CowNo cowNo = passport.GetCowNo();

	if(cowNo.IsNull())
		return;

	BucketEntry* pEntry = find(cowNo);
	delete pEntry;

	m_entries.push_back(new BucketEntry(passport));
}

