#ifndef __UNIQUEPRESENCEKEY_H__
#define __UNIQUEPRESENCEKEY_H__

#include <set>
#include <stdint.h>
#include <logic\herdregistry\CowPresenceKey.h>

class UniquePresenceKeys 
{	
public:	
	typedef std::set<CowPresenceKey> keys_container;
	typedef keys_container::const_iterator const_iterator;
	typedef keys_container::iterator iterator;
		
	UniquePresenceKeys();
	UniquePresenceKeys(UniquePresenceKeys&& ids);
	UniquePresenceKeys(const UniquePresenceKeys& ids);

	void add(const CowPresenceKey& key);
	void remove(const CowPresenceKey& key);	

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();

private:
	keys_container m_keys;
};


#endif