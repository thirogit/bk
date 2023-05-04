#ifndef __DEDUCTIONELEMENTVIEW_H__
#define __DEDUCTIONELEMENTVIEW_H__

#include <data\runtime\ItemId.h>
#include <types\NullDecimal.h>

struct DeductionElementViewMember
{
	enum
	{
		Enabled,
		Code,
		Fraction,
		Reason,
		DeductedAmount
	};
};


class DeductionElementView 
{
public:
	DeductionElementView(ItemId id);
		
	const ItemId& GetItemId() const;
	
	const std::wstring& 	GetCode() const;
	const std::wstring& 	GetReason() const;
	const Decimal&	GetFraction() const;
	bool GetIsEnabled() const;
	const NullDecimal&	GetAmount() const;

	void SetCode(const std::wstring& sCode);
	void SetReason(const std::wstring& sReason);
	void SetFraction(const Decimal&	fraction);
	void SetIsEnabled(bool bEnabled);
	void SetAmount(const NullDecimal& amount);
		
private:
	ItemId m_id;
	bool m_enabled;
	std::wstring m_code;
	Decimal m_fraction;
	std::wstring m_reason;
	NullDecimal m_amount;
	
};

#endif