#ifndef __PBSFORMATTER_H__
#define __PBSFORMATTER_H__

#include "Formatter.h"
#include <xercesc/dom/DOM.hpp>

class PBSFormatter : public Formatter
{
public:
	PBSFormatter();	
	virtual std::wstring GetFormatterName();
	virtual std::wstring GetFileExtension();
	virtual void FormatTransfers(  MoneyTransferEntryRange moneyTransfers,
										MoneyTransferContext* pContext,
										CFile& outputFile,
										SimpleProgress *progress);
	virtual ~PBSFormatter();
private:
	void SerializeToXml(MoneyTransferContext* pContext,MoneyTransferEntry* pEntry,xercesc::DOMElement * pParentElement);


};

#endif