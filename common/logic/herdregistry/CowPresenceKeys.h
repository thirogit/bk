#ifndef __COWPRESENCEKEYS_H__
#define __COWPRESENCEKEYS_H__

#include "CowPresenceKey.h"
#include <set>

class CowPresenceKeys
{
public:

	typedef std::set<CowPresenceKey> cowpresencekey_set;
	typedef cowpresencekey_set::const_iterator const_iterator;
	typedef cowpresencekey_set::iterator iterator;
	
	CowPresenceKeys();
	~CowPresenceKeys();
	CowPresenceKeys(const CowPresenceKeys& src);
	CowPresenceKeys(CowPresenceKeys&& src);
		
	void add(const CowPresenceKey& key);	

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();
	
private:
	cowpresencekey_set m_keys;
};


#endif 
