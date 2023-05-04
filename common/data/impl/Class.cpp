#include "stdafx.h"
#include "Class.h"

Class::Class(uint32_t Id /*= NULL_ID*/) : m_classid(Id),m_ruleId(NULL_ID)
{	

}

Class::Class(const IClass* pSrc)
{
	m_classid = pSrc->GetId();
	CopyFrom(pSrc);
}

uint32_t Class::GetId() const
{
	return m_classid;
}

const std::wstring&	Class::GetClassNm() const
{
	return m_classname;
}

void Class::SetClassName(const std::wstring& sClassName)
{
	m_classname = sClassName;
}


const std::wstring&	Class::GetClassCode() const
{
	return m_classcode;
}

void Class::SetClassCode(const std::wstring& sStockCode)
{
	m_classcode = sStockCode;
}


const NullDecimal& Class::GetMyPerKgPrice() const
{
	return m_myperkgprice;
}

void Class::SetMyPerKgPrice(const NullDecimal& myPerKgPrice)
{
	m_myperkgprice = myPerKgPrice;
}


const CowSex& Class::GetPredefSex() const
{
	return m_predefsex;
}

void Class::SetPredefSex(const CowSex& cowsex)
{
	m_predefsex = cowsex;
}

uint32_t Class::GetRuleId() const
{
	return m_ruleId;
}

void Class::SetRule(uint32_t ruleId)
{
	m_ruleId = ruleId;
}


void Class::CopyFrom(const IClass* pSrc)
{	
	SetClassName(pSrc->GetClassNm());
	SetClassCode(pSrc->GetClassCode());
	SetMyPerKgPrice(pSrc->GetMyPerKgPrice());
	SetPredefSex(pSrc->GetPredefSex());	
	SetRule(pSrc->GetRuleId());

}
