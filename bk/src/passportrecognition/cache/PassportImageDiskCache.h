#ifndef __PASSPORTIMAGEDISKCACHE_H__
#define __PASSPORTIMAGEDISKCACHE_H__

#include "../../image/fipWinImageEx.h"
#include <data\runtime\ItemId.h>
#include <map>

class PassportImageDiskCache
{
public:	
	PassportImageDiskCache();
	~PassportImageDiskCache();
	void CachePassportImage(ItemId id,const fipWinImageEx&);
	fipWinImageEx GetPassportImage(ItemId id);
	bool IsCached(ItemId id) const;
	void Purge();
protected:
	HANDLE CreateImageFile(ItemId id);
private:
	std::map<ItemId, HANDLE> m_files;
};



#endif 
