#ifndef __MONEYTRANSFERVIEW_H__
#define __MONEYTRANSFERVIEW_H__

#include <string>
#include <stdint.h>
#include <types\Decimal.h>
#include "../../../view/ColoredObject.h"

struct MoneyTransferViewMember
{
	enum
	{
		MoneyTransferViewMember_InvoiceId,
		MoneyTransferViewMember_HentAlias,
		MoneyTransferViewMember_InvoiceNo,
		MoneyTransferViewMember_CustomInvoiceNo,
		MoneyTransferViewMember_TransferValue,
		MoneyTransferViewMember_StatusDesc
	};
};

class MoneyTransferView
{
public:
	MoneyTransferView(uint32_t invoiceId);
	
	uint32_t GetInvoiceId() const;
	
	void SetHentAlias(const std::wstring& hentAlias);
	void SetInvoiceNo(const std::wstring& invoiceNo);
	void SetCustomInvoiceNo(const std::wstring& customInvoiceNo);
	void SetTransferValue(const Decimal& transferValue);
	void SetStatusDesc(const ColoredObject<std::wstring>& statusDesc);
	
	const std::wstring& GetHentAlias() const;
	const std::wstring& GetInvoiceNo() const;
	const std::wstring& GetCustomInvoiceNo()  const;
	const Decimal& GetTransferValue()  const;
	const ColoredObject<std::wstring>& GetStatusDesc()  const;
	
private:
	uint32_t m_invoiceId;
	std::wstring m_hentAlias;
	std::wstring m_invoiceNo;
	std::wstring m_customInvoiceNo;
	Decimal m_transferValue;
	ColoredObject<std::wstring> m_statusDesc;	
};


#endif