#ifndef __UNIQUEIDS_H__
#define __UNIQUEIDS_H__

#include <set>
#include <stdint.h>

class UniqueIds 
{	
public:
	
	typedef std::set<uint32_t> ids_container;
	typedef ids_container::const_iterator const_iterator;
	typedef ids_container::iterator iterator;
		
	UniqueIds();
	UniqueIds(UniqueIds&& ids);
	UniqueIds(const UniqueIds& ids);
	UniqueIds& operator=(const UniqueIds& right);


	void add(uint32_t id);
	void remove(uint32_t id);
	void clear();
	

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();

private:
	ids_container m_ids;
};


#endif