#ifndef __DEDUCTIONDEFINITIONVIEW_H__
#define __DEDUCTIONDEFINITIONVIEW_H__

#include <data\runtime\ItemId.h>
#include <types\NullDecimal.h>

struct DeductionDefinitionViewMember
{
	enum
	{
		Enabled,
		Code,
		Fraction,
		Reason
	};
};


class DeductionDefinitionView 
{
public:
	DeductionDefinitionView(ItemId id);
		
	const ItemId& GetItemId() const;
	
	const std::wstring& 	GetCode() const;
	const std::wstring& 	GetReason() const;
	const Decimal&	GetFraction() const;
	bool GetIsEnabled() const;
	
	void SetCode(const std::wstring& sCode);
	void SetReason(const std::wstring& sReason);
	void SetFraction(const Decimal&	fraction);
	void SetIsEnabled(bool bEnabled);
		
private:
	ItemId m_id;
	bool m_enabled;
	std::wstring m_code;
	Decimal m_fraction;
	std::wstring m_reason;
	
};

#endif