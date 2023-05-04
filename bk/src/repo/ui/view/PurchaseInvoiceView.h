#ifndef __PURCHASEINVOICEVIEW_H__
#define __PURCHASEINVOICEVIEW_H__

#include "PurchaseHentView.h"
#include <data\PayWay.h>
#include <data\InvoiceType.h>
#include <types\Decimal.h>

struct PurchaseInvoiceViewMember
{
	enum : uint32_t
	{		
		CustomNumber,
		InvoiceDate,
		InvoiceType,
		PayWay,
		Hent,
		CowCount,
		VatRate,
		TotalNet,
	};
};

class PurchaseInvoiceView 
{
public:
	PurchaseInvoiceView(const std::wstring& id);

	const std::wstring& GetInvoiceId() const;
		
	const DateTime& GetInvoiceDate() const;
	const Decimal& GetVatRate() const;
	PayWay GetPayWay() const;
	const std::wstring& GetCustomNumber() const;	
	const PurchaseHentView* GetInvoiceHent() const;
	InvoiceType GetInvoiceType() const;
	int GetCowCount() const ;
	const Decimal& GetTotalNet() const;
	
	void SetInvoiceDate(const DateTime& invoiceDt);
	void SetVatRate(const Decimal& vatRate);
	void SetPayWay(PayWay payWay);
	void SetCustomNumber(const std::wstring& customNumber);	
	void SetInvoiceHent(const PurchaseHentView* invoiceHent);
	void SetInvoiceType(InvoiceType invoiceType);
	void SetCowCount(int cowCount);
	void SetTotalNet(const Decimal& totalNet);
	
private:

	DateTime m_InvoiceDate;
	Decimal m_VatRate;
	PayWay m_PayWay;
	std::wstring m_CustomNumber;	
	PurchaseHentView m_InvoiceHent;
	InvoiceType m_InvoiceType;
	int m_CowCount;
	Decimal m_TotalNet;
	std::wstring m_invoiceId;
};

#endif