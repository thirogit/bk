#ifndef __FORMATTER_H__
#define __FORMATTER_H__

#include <progress/SimpleProgress.h>
#include "../MoneyTransferEntry.h"
#include "../MoneyTransferContext.h"
#include <boost\range\any_range.hpp>
#include <fstream>

typedef boost::any_range<MoneyTransferEntry*, boost::random_access_traversal_tag, MoneyTransferEntry*, std::ptrdiff_t> MoneyTransferEntryRange;

class Formatter
{
public:
	virtual std::wstring GetFormatterName() = 0;
	virtual std::wstring GetFileExtension() = 0;
	virtual std::string GetDefaultEncoding() = 0;

	virtual void FormatTransfers(  MoneyTransferEntryRange moneyTransfers,
										MoneyTransferContext* pContext,
										std::wofstream& output,
										SimpleProgress *progress) = 0;
	virtual ~Formatter() {};
};

#endif