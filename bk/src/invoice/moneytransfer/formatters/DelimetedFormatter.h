#ifndef __DELIMETEDFORMATTER_H__
#define __DELIMETEDFORMATTER_H__

#include "Formatter.h"
#include "fields/FormatterField.h"
#include <progress/SimpleProgress.h>
#include <list>

class DelimetedFormatter : public Formatter
{
public:
	DelimetedFormatter(wchar_t cDelimiter,const std::wstring& sExtension,
									   const std::wstring& sFormatterName);
	virtual ~DelimetedFormatter();
	void AddField(FormatterField* pField);

	virtual std::wstring GetFormatterName();
	virtual std::wstring GetFileExtension();
	virtual std::string GetDefaultEncoding();
	virtual void FormatTransfers(  MoneyTransferEntryRange moneyTransfers,
										MoneyTransferContext* pContext,
										std::wofstream& output,
										SimpleProgress *progress);
protected:
	void WriteLineEnd(std::wofstream& output);
	void WriteDelimiter(std::wofstream& output);
protected:
	std::list<FormatterField*> m_fields;
	wchar_t m_cDelimiter;
	std::wstring m_sExtension;
	std::wstring m_sFormatterName;

};

#endif