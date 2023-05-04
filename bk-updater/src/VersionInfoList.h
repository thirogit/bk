#ifndef __VERSIONINFOLIST_H__
#define __VERSIONINFOLIST_H__

#include <arrays\SortArray.h>
#include <versioning\VersionInfo.h>

class CVersionInfoList : public CSortArray<CVersionInfo> 
{
private:
	class VersionComparator : public CSortArray<CVersionInfo>::SortArrayComparator
	{
	public:
		VersionComparator(bool bAsc);
		COMPARE_RESULT Compare(CVersionInfo *DataLeft,CVersionInfo *DataRight);		
	};
public:
	void Sort(bool bAscending);
};




#endif