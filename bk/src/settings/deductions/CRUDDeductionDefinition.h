#pragma once

#include "CRUDObject.h"
#include <boost\range\any_range.hpp>
#include <types\Decimal.h>
#include <configuration/deductions/DeductionDefinition.h>

class CRUDDeductionDefinition : public CRUDObject
{
public:
	CRUDDeductionDefinition(ItemId id,const DeductionDefinition* object);
	CRUDDeductionDefinition(ItemId id);
	~CRUDDeductionDefinition();

	const std::wstring& 	GetCode() const;
	const std::wstring& 	GetReason() const;
	const Decimal&	GetFraction() const;
	bool GetIsEnabled() const;
	const DeductionDefinition* Obj() const;

	void SetCode(const std::wstring& sCode);
	void SetReason(const std::wstring& sReason);
	void SetFraction(const Decimal&	fraction);
	void SetIsEnabled(bool bEnabled);

private:
	DeductionDefinition* m_object;
};

typedef boost::any_range<CRUDDeductionDefinition*, boost::forward_traversal_tag, CRUDDeductionDefinition*, std::ptrdiff_t> CRUDDeductionDefinitionRange;

