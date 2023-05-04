#ifndef __MONEYTRANSFERTEXT_H__
#define __MONEYTRANSFERTEXT_H__

#include "../view/MoneyTransferView.h"

class MoneyTransferEntryText 
{
public:
	virtual std::wstring Text(MoneyTransferView *view,uint32_t memberId);

};

#endif