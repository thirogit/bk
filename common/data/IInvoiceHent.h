#ifndef __IINVOICEHENT_H__
#define __IINVOICEHENT_H__

#include "types/AccountNo.h"
#include "types/HentNo.h"
#include "types/DateTime.h"

class IInvoiceHent
{
public:
	virtual const std::wstring& GetName() const = 0;
	virtual const std::wstring& GetStreet() const = 0;
	virtual const std::wstring& GetPOBox() const = 0;
	virtual const std::wstring& GetCity() const = 0;
	virtual const std::wstring& GetZip() const = 0;
	virtual const std::wstring& GetFiscalNo() const = 0;
	virtual const HentNo& GetHentNo() const = 0;
	virtual const std::wstring& GetPESEL() const = 0;
	virtual const std::wstring& GetIdNo() const = 0;
	virtual const DateTime& GetIssueDate() const = 0;
	virtual const std::wstring& GetIssuePost() const = 0;
	virtual const AccountNo& GetAccountNo() const = 0;
	virtual const std::wstring& GetBankName() const = 0;	
	virtual uint32_t GetParentHentId() const = 0;
};


#endif
