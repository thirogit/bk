#ifndef __PURCHASEINVIOCEDEDUCTION_H__
#define __PURCHASEINVIOCEDEDUCTION_H__

#include "IPurchaseInvoiceDeduction.h"

class SerializablePurchaseInvoiceDeduction;

class PurchaseInvoiceDeduction : public IPurchaseInvoiceDeduction
{
public:
	PurchaseInvoiceDeduction(const std::wstring& id);
	void CopyFrom(IPurchaseInvoiceDeduction* src);

	virtual const std::wstring& GetId() const;
	virtual const std::wstring& GetCode() const;	
	virtual const std::wstring& GetReason() const;	
	virtual const Decimal& GetFraction() const;	
	virtual bool GetIsEnabled() const;

	void SetCode(const std::wstring& sCode);
	void SetReason(const std::wstring& sReason);
	void SetFraction(const Decimal&	fraction);
	void SetIsEnabled(bool bEnabled);

private:
	std::wstring m_id;
	std::wstring m_code;
	std::wstring m_reason;
	Decimal	m_fraction;
	bool m_enabled;

	friend class SerializablePurchaseInvoiceDeduction;
};


#endif
