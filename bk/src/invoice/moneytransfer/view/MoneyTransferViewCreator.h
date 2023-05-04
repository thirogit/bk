#ifndef __MONEYTRANSFERVIEWCREATOR_H__
#define __MONEYTRANSFERVIEWCREATOR_H__

#include "MoneyTransferView.h"
#include "../MoneyTransferEntry.h"

class MoneyTransferViewCreator  
{
public:
	MoneyTransferViewCreator();
	MoneyTransferView* CreateView(const MoneyTransferEntry* entry);
	void RefreshView(MoneyTransferView* pView, const MoneyTransferEntry* entry);

};


#endif