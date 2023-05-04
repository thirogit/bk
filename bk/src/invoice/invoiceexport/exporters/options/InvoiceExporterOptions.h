#ifndef __INVOICEEXPORTEROPTIONS_H__
#define __INVOICEEXPORTEROPTIONS_H__

#include "InvoiceExporterOption.h"
#include <map>
#include <arrays\Enumerator.h>
#include <CommonTypes.h>

class InvoiceExporterOptions 
{
public:
	InvoiceExporterOptions();
	InvoiceExporterOptions(const InvoiceExporterOptions& src);
	~InvoiceExporterOptions();
	
	InvoiceExporterOptions& operator=(const InvoiceExporterOptions& src);

	
	InvoiceExporterOption* GetOption(const std::wstring& optName) const;
	InvoiceExporterOption* AddOption(const std::wstring& optName);
	void AddOption(const InvoiceExporterOption& option);
	void RemoveOption(const std::wstring& optName);
	PtrEnumerator<InvoiceExporterOption> EnumOptions() const;
	count_t GetOptionsCount() const;


	
protected:
	void AddOption(InvoiceExporterOption* option);
	
private:
	std::map<std::wstring,InvoiceExporterOption*> m_options;	

};

#endif