#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../InvoiceHent.h"


class SerializableInvoiceHent
{
public:
	SerializableInvoiceHent(InvoiceHent& pHent);
	void serialize(SF::Archive &archive);
protected:

	std::wstring& Name();
	std::wstring& City();
	std::wstring& ZipCode();
	std::wstring& Street();
	std::wstring& POBox();
	
	std::wstring& FiscalNumber();
	AccountNo&   AccountNumber();
	HentNo&      HentNumber();	
	std::wstring& PersonalNo();
	std::wstring& PersonalIdNo();
	DateTime&    IssueDate();
	std::wstring& IssuePost();
	std::wstring& BankName();
	uint32_t&    ParentHentId();	

private:
	InvoiceHent& m_Hent;

	
};
