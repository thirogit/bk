#ifndef __INVOICEEXPORTEROPTION_H__
#define __INVOICEEXPORTEROPTION_H__

#include <string>

class InvoiceExporterOption
{
public:
	InvoiceExporterOption(const InvoiceExporterOption& src);
	InvoiceExporterOption(const std::wstring& sName);

	const std::wstring& name() const;	
	const std::wstring& get() const;	

	void set(const std::wstring& str);	
private:
	std::wstring m_Name;
	std::wstring m_Value;
};

#endif