#include "stdafx.h"
#include "PassportImageDiskCache.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PassportImageDiskCache::PassportImageDiskCache()
{
}

PassportImageDiskCache::~PassportImageDiskCache()
{
	Purge();
}

void PassportImageDiskCache::CachePassportImage(ItemId id, const fipWinImageEx&)
{
	int buffSize = ::GetTempPath(0,NULL);
	
	

}

fipWinImageEx PassportImageDiskCache::GetPassportImage(ItemId id)
{
	return std::move(fipWinImageEx());
}

bool PassportImageDiskCache::IsCached(ItemId id) const
{
	return m_files.find(id) != m_files.end();
}

HANDLE PassportImageDiskCache::CreateImageFile(ItemId id)
{
	return NULL;
}

void PassportImageDiskCache::Purge()
{
}