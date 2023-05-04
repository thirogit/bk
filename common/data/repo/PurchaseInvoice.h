#ifndef __PURCHASEINVOICE_H__
#define __PURCHASEINVOICE_H__

#include "IPurchaseInvoice.h"
#include "PurchaseCow.h"
#include "PurchaseInvoiceDeduction.h"
#include "PurchaseHent.h"
#include <data/PayWay.h>
#include <arrays/Enumerator.h>
#include <arrays/IdableIndex.h>
#include <arrays/StringIdIndex.h>
#include <utils/IdGenerator.h>

class SerializablePurchaseInvoice;

class PurchaseInvoice : public IPurchaseInvoice
{
public:	
	PurchaseInvoice(const std::wstring& id);
	~PurchaseInvoice();
	
	void CopyFrom(const IPurchaseInvoice* src);

	const std::wstring& GetId() const;

	const DateTime& GetInvoiceDate() const;
	const DateTime& GetTransactionDate() const;	
	const std::wstring& GetTransactionPlace() const;	
	const Decimal& GetVatRate() const;
	PayWay GetPayWay() const;
	const std::wstring& GetCustomNumber() const;	
	int GetPayDueDays() const;	
	IPurchaseHent* GetInvoiceHent() const;		
	InvoiceType GetInvoiceType() const;
	virtual PtrEnumerator<IPurchaseCow> EnumCows() const;
	virtual PtrEnumerator<IPurchaseInvoiceDeduction> EnumDeductions() const;

	void SetInvoiceDate(const DateTime& invoiceDt);
	void SetTransactionDate(const DateTime& transactionDt);	
	void SetTransactionPlace(const std::wstring& sTransactionPlace);
	void SetVatRate(const Decimal& vatRate);
	void SetPayWay(PayWay payWay);
	void SetCustomNumber(const std::wstring& sCustomNumber);
	void SetPayDueDays(int payDueDays);	
	void SetInvoiceHent(IPurchaseHent* pInvoiceHent);
	void SetInvoiceType(InvoiceType invoiceType);

	int GetCowCount() const;
	PurchaseCow* GetCowAt(int iCow);
	const IPurchaseCow* GetCowAt(int iCow) const;

	PurchaseCow* GetCow(const std::wstring& id);
	IPurchaseCow* GetCow(const std::wstring& id) const;

	PurchaseCow* AddCow(IPurchaseCow* pCow);
	PurchaseCow* AddCow();	

	int GetDeductionsCount() const;
	PurchaseInvoiceDeduction* GetDeductionAt(int index);
	const IPurchaseInvoiceDeduction* GetDeductionAt(int index) const;

	PurchaseInvoiceDeduction* AddDeduction(IPurchaseInvoiceDeduction* pDeduction);
	PurchaseInvoiceDeduction* AddDeduction();

private:
	std::wstring  ObtainNewCowId() const;
	std::wstring  ObtainNewDeductionId() const;
private:
	std::wstring m_id;
	DateTime m_InvoiceDate;
	DateTime m_TransactionDate;	
	std::wstring m_TransactionPlace;
	Decimal m_VatRate;
	PayWay m_PayWay;
	std::wstring m_CustomNumber;
	int m_PayDueDays;	
	PurchaseHent m_InvoiceHent;
	InvoiceType m_InvoiceType;

	IdGenerator m_idGenerator;

	StirngIdIndex<PurchaseCow> m_Cows;
	StirngIdIndex<PurchaseInvoiceDeduction> m_Deductions;

	friend class SerializablePurchaseInvoice;
};
	

#endif
