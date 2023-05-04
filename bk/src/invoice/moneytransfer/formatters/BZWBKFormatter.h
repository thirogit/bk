#ifndef __BZWBKFORMATTER_H__
#define __BZWBKFORMATTER_H__

#include "DelimetedFormatter.h"

class BZWBKFormatter : public DelimetedFormatter
{
public:
	BZWBKFormatter();
	virtual void FormatTransfers(MoneyTransferEntryRange moneyTransfers,
										MoneyTransferContext* pContext,
										std::wofstream& output,
										SimpleProgress *progress);

	virtual std::string GetDefaultEncoding();
};

#endif