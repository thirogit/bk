#ifndef __CLASS_H__
#define __CLASS_H__

#include "../IClass.h"

class SerializableClass;

class Class : public IClass
{
public:
	explicit Class(uint32_t Id = NULL_ID);
	Class(const IClass* pSrc);

	void CopyFrom(const IClass* pSrc);
	
	uint32_t GetId() const;
	virtual const std::wstring& 	GetClassNm() const;	
	virtual const std::wstring& 	GetClassCode() const;	
	virtual const NullDecimal&	GetMyPerKgPrice() const;	
	virtual const CowSex&	    GetPredefSex() const;	
	virtual uint32_t			GetRuleId() const;
		
	void	SetClassName(const std::wstring& sClassName);
	void	SetClassCode(const std::wstring& sClassCode);
	void	SetMyPerKgPrice(const NullDecimal& myPerKgPrice);
	void	SetPredefSex(const CowSex& cowsex);	
	void	SetRule(uint32_t ruleId);
	
protected:
	std::wstring	m_classname;
	std::wstring	m_classcode;
	uint32_t	m_classid;
	uint32_t	m_ruleId;
	NullDecimal	m_myperkgprice;
	CowSex	    m_predefsex;

	friend class SerializableClass;
};


#endif