#ifndef __DEDUCTIONDEFINITION_H__
#define __DEDUCTIONDEFINITION_H__

#include "DeductionDetails.h"
#include "IDeductionDefinition.h"

class SerializableDeductionDefinition;

class DeductionDefinition : public IDeductionDefinition
{
public:
	DeductionDefinition(uint32_t id);
	DeductionDefinition(const IDeductionDefinition* pSrc);
	
	virtual void CopyFrom(const IDeductionDefinition* pSrc);
	void SetFrom(const DeductionDetails& details);
	const DeductionDetails& GetDetails() const;

	uint32_t	GetId() const;
	virtual const std::wstring& 	GetCode() const;
	virtual const std::wstring& 	GetReason() const;
	virtual const Decimal&	GetFraction() const;
	virtual bool GetIsEnabledByDefault() const;

	void SetCode(const std::wstring& sCode);
	void SetReason(const std::wstring& sReason);
	void SetFraction(const Decimal&	fraction);
	void SetIsEnabledByDefault(bool bEnabled);
private:
	DeductionDetails m_details;
	bool m_enabledByDefault;
	uint32_t m_Id;
	
	friend class SerializableDeductionDefinition;
};


#endif
