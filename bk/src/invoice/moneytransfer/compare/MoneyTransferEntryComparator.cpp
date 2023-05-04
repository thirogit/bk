#include "stdafx.h"
#include "MoneyTransferEntryComparator.h"


MonenyTansferEntryComparator::MonenyTansferEntryComparator(bool bAsc) : CSortArray<MoneyTransferEntry>::SortArrayComparator(bAsc)
{
}

COMPARE_RESULT MonenyTansferEntryComparator::Compare(MoneyTransferEntry *DataLeft,MoneyTransferEntry *DataRight)
{
	return COMPARE_LESS;	
}
