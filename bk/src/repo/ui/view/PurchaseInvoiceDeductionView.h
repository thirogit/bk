#ifndef __PURCHASEINVOICEDEDUCTIONVIEW_H__
#define __PURCHASEINVOICEDEDUCTIONVIEW_H__

#include <stdint.h>
#include <string>
#include <types\Decimal.h>

struct PurchaseInvoiceDeductionViewMember
{
	enum : uint32_t
	{		
		Code,
		Reason,
		Fraction,
		Enabled
	};
};

class PurchaseInvoiceDeductionView
{
public:
	PurchaseInvoiceDeductionView(const std::wstring& id);

	const std::wstring& GetId() const;
	
	const std::wstring& GetCode() const;
	const std::wstring& GetReason() const;
	const Decimal& GetFraction() const;
	bool GetIsEnabled() const;

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
	
};

#endif