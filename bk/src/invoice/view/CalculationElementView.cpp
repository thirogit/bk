#include "stdafx.h"
#include "CalculationElementView.h"

CalculationElementView::CalculationElementView(ItemId id) : m_id(id)
{
}
	
const ItemId& CalculationElementView::GetItemId() const
{
	return m_id;
}
	
const CowNo& CalculationElementView::GetCowNo() const
{
	return m_cowNo;
}

bool   CalculationElementView::GetHasTerminalValues() const
{
	return m_hasTerm;		   
}
	   
const ColoredObject<std::wstring>& CalculationElementView::GetClass() const
{
	return m_ClassCd;
}

const ColoredObject<Decimal>& CalculationElementView::GetWeight() const
{
	return m_Weight;
}

const ColoredObject<NullDecimal>& CalculationElementView::GetOtherPrice() const
{
	return m_OtherPrice;
}

const std::wstring& CalculationElementView::GetOtherHentAlias() const
{
	return m_OtherHentAlias;
}

const ColoredObject<std::wstring>& CalculationElementView::GetRule() const
{
	return m_RuleCd;	
}
	
const ColoredObject<Decimal>& CalculationElementView::GetNetPricePerKg() const
{
	return m_NetPricePerKg;
}

const ColoredObject<Decimal>& CalculationElementView::GetNetPrice() const
{
	return m_NetPrice;
}

const ColoredObject<Decimal>& CalculationElementView::GetTaxValue() const
{
	return m_TaxValue;	
}

const ColoredObject<Decimal>& CalculationElementView::GetGrossPrice() const
{
	return m_GrossPrice;
}


void CalculationElementView::SetCowNo(const CowNo& cowNo)
{
	m_cowNo = cowNo;
}
	
void CalculationElementView::SetHasTerminalValues(bool bHasTerm)
{
	m_hasTerm = bHasTerm;
}
		   
void CalculationElementView::SetClass(const ColoredObject<std::wstring>& classCd)
{
	m_ClassCd = classCd;
}

void CalculationElementView::SetWeight(const ColoredObject<Decimal>& weight)
{
	m_Weight = weight;
}

void CalculationElementView::SetOtherPrice(const ColoredObject<NullDecimal>& otherPrice)
{
	m_OtherPrice = otherPrice;
}

void CalculationElementView::SetOtherHentAlias(const std::wstring& sAlias)
{
	m_OtherHentAlias = sAlias;
}

void CalculationElementView::SetRule(const ColoredObject<std::wstring>& ruleCd)
{
	m_RuleCd = ruleCd;	
}
	
void CalculationElementView::SetNetPricePerKg(const ColoredObject<Decimal>& netPricePerKg)
{
	m_NetPricePerKg = netPricePerKg;
}

void CalculationElementView::SetNetPrice(const ColoredObject<Decimal>& netPrice)
{
	m_NetPrice = netPrice;	
}

void CalculationElementView::SetTaxValue(const ColoredObject<Decimal>& taxValue)
{
	m_TaxValue = taxValue;
}

void CalculationElementView::SetGrossPrice(const ColoredObject<Decimal>& grossPrice)
{
	m_GrossPrice = grossPrice;
}

