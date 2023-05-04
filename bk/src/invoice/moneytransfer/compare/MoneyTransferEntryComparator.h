#ifndef __MONEYTRANSFERENTRYCOMPARATOR_H__
#define __MONEYTRANSFERENTRYCOMPARATOR_H__

#include "../MoneyTransferEntry.h"
#include "../../datatypes/DataComparator.h"

enum MONEYTRANSFERENTRY_MEMBERS
{
	MONEYTRANSFERENTRY_HENT,
	MONEYTRANSFERENTRY_INVOICENO,
	MONEYTRANSFERENTRY_INVOICECUSTNO,
	MONEYTRANSFERENTRY_INVOICEVALUE,
	MONEYTRANSFERENTRY_INVALIDCAUSE,
	MONEYTRANSFERENTRY_TRANSFERVALUE,
	MONEYTRANSFERENTRY_MAX
};


class MonenyTansferEntryComparator : public CSortArray<MoneyTransferEntry>::SortArrayComparator
{
public:
	MonenyTansferEntryComparator(bool bAsc);
	virtual COMPARE_RESULT Compare(MoneyTransferEntry *DataLeft,MoneyTransferEntry *DataRight);
};


#endif