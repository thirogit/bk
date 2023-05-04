#ifndef __INVOICEHENT_H__
#define __INVOICEHENT_H__

#include "../IInvoiceHent.h"
#include "../IHent.h"
#include  <casajson/json.h>

class SerializableInvoiceHent;

class InvoiceHent : public IInvoiceHent
{
public:
	InvoiceHent();		
	void CopyFrom(const InvoiceHent& src);
	void CopyFrom(const IInvoiceHent* src);
	void CopyFrom(const IHent* src);
			
	virtual const std::wstring& GetName() const;
	void SetName(const std::wstring& sName);

	virtual const std::wstring& GetStreet() const;
	void SetStreet(const std::wstring& sStreet);
		
	virtual const std::wstring& GetPOBox() const;
	void SetPOBox(const std::wstring& sPOBox);
	
	virtual const std::wstring& GetCity() const;
	void SetCity(const std::wstring& sCity);
	
	virtual const std::wstring& GetZip() const;
	void SetZip(const std::wstring& sZip);

	virtual const std::wstring& GetFiscalNo() const;
	void SetFiscalNo(const std::wstring& sFiscalNo);

	virtual const HentNo& GetHentNo() const;
	void SetHentNo(const HentNo& hentNo);
	
	virtual const std::wstring& GetPESEL() const;
	void SetPESEL(const std::wstring& sPESEL);
		
	virtual const std::wstring& GetIdNo() const;
	void SetIdNo(const std::wstring& sIdNo);
		
	virtual const DateTime& GetIssueDate() const;
	void SetIssueDate(const DateTime& IdIssueDate);
		
	virtual const std::wstring& GetIssuePost() const;
	void SetIssuePost(const std::wstring& sIssuePost);

	virtual const AccountNo& GetAccountNo() const;
	void SetAccountNo(const AccountNo& accountNo);
	
	virtual const std::wstring& GetBankName() const;
	void SetBankName(const std::wstring& sBankName);
	
	virtual uint32_t GetParentHentId() const;
	void SetParentHentId(uint32_t parentHentId);

	static InvoiceHent FromJson(const json::value& value);
	json::value ToJson() const;

protected:
	std::wstring m_name;
	std::wstring m_street;
	std::wstring m_pobox;
	std::wstring m_city;
	std::wstring m_zip;
	std::wstring m_nip;
	HentNo m_hentno;
	std::wstring m_pesel;
	std::wstring m_idno;
	DateTime m_issuedate;
	std::wstring m_issuepost;
	AccountNo m_accountno;
	std::wstring m_bankname;
	uint32_t m_parentHentId;
	
	friend class SerializableInvoiceHent;
};



#endif