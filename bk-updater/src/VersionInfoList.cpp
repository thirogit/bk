#include "stdafx.h"
#include "VersionInfoList.h"


CVersionInfoList::VersionComparator::VersionComparator(bool bAsc) :  
	CSortArray<CVersionInfo>::SortArrayComparator (bAsc)
{
}

COMPARE_RESULT CVersionInfoList::VersionComparator::Compare(CVersionInfo *DataLeft,CVersionInfo *DataRight)
{
	return DataLeft->GetVerNo() - DataRight->GetVerNo();
}

void CVersionInfoList::Sort(bool bAscending)
{
	CSortArray<CVersionInfo>::Sort(CVersionInfoList::VersionComparator(bAscending));
}