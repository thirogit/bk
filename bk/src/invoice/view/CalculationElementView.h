#ifndef __CALCULATIONELEMENTVIEW_H__
#define __CALCULATIONELEMENTVIEW_H__

#include <data\runtime\ItemId.h>
#include <data\types\CowNo.h>
#include <types\Decimal.h>
#include <types\NullDecimal.h>
#include "../../view/ColoredObject.h"

struct CalculationElementViewMember
{
	enum
	{
		CalculationElementView_HasTerm,
		CalculationElementView_CowNo,
		CalculationElementView_Class,
		CalculationElementView_Weight,
		CalculationElementView_OtherHent,
		CalculationElementView_OtherPrice,
		CalculationElementView_Rule,
		CalculationElementView_NetPricePerKg,
		CalculationElementView_NetPrice,
		CalculationElementView_TaxValue,
		CalculationElementView_GrossPrice		
	};
};


class CalculationElementView 
{
public:
	CalculationElementView(ItemId id);	
		
	const ItemId& GetItemId() const;	
	const CowNo& GetCowNo() const;	
		  bool   GetHasTerminalValues() const;
		   
	const ColoredObject<std::wstring>& GetClass() const;
	const ColoredObject<Decimal>& GetWeight() const;
	const ColoredObject<NullDecimal>& GetOtherPrice() const;
	const std::wstring& GetOtherHentAlias() const;
	const ColoredObject<std::wstring>& GetRule() const;	
	const ColoredObject<Decimal>& GetNetPricePerKg() const;
	const ColoredObject<Decimal>& GetNetPrice() const;
	const ColoredObject<Decimal>& GetTaxValue() const;
	const ColoredObject<Decimal>& GetGrossPrice() const;
	
	void SetCowNo(const CowNo& cowNo);	
	void SetHasTerminalValues(bool bHasTerminalValues);		   
	void SetClass(const ColoredObject<std::wstring>& sClassCd);
	void SetWeight(const ColoredObject<Decimal>& weight);
	void SetOtherPrice(const ColoredObject<NullDecimal>& otherPrice);
	void SetOtherHentAlias(const std::wstring& sAlias);
	void SetRule(const ColoredObject<std::wstring>& sRuleCd);	
	void SetNetPricePerKg(const ColoredObject<Decimal>& netPricePerKg);
	void SetNetPrice(const ColoredObject<Decimal>& netPricce);
	void SetTaxValue(const ColoredObject<Decimal>& taxValue);
	void SetGrossPrice(const ColoredObject<Decimal>& grossPrice);
		
private:
	ItemId m_id;
	CowNo m_cowNo;
	bool  m_hasTerm;		   
	ColoredObject<std::wstring> m_ClassCd;
	ColoredObject<Decimal> 		m_Weight;
	ColoredObject<NullDecimal>	m_OtherPrice;
	std::wstring 				m_OtherHentAlias;
	ColoredObject<std::wstring> m_RuleCd;	
	ColoredObject<Decimal>		m_NetPricePerKg;
	ColoredObject<Decimal>		m_NetPrice;
	ColoredObject<Decimal>		m_TaxValue;
	ColoredObject<Decimal>		m_GrossPrice;
	
};

#endif