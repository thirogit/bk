#include "stdafx.h"
#include "DocItemView.h"


DocItemView::DocItemView(ItemId id) : m_id(id)
{
}

const ItemId& DocItemView::GetItemId() const
{
	return m_id;
}
	
const CowNo& DocItemView::GetCowNo() const
{
	return m_cowNo;
}

const std::wstring& DocItemView::GetPassNo() const
{
	return m_passNo;
}

const std::wstring& DocItemView::GetFstOwnerAlias() const
{
	return m_fstOwnerAlias;
}

const DateTime& DocItemView::GetBirthDate() const
{
	return m_birthDt;
}

const ColoredObject<Decimal>& DocItemView::GetWeight() const
{
	return m_weight;
}

const CowSex& DocItemView::GetSex() const 
{
	return m_sex;
}

const ColoredObject<std::wstring>& DocItemView::GetStock() const
{
	return m_stock;
}

const ColoredObject<std::wstring>& DocItemView::GetClass() const
{
	return m_class;
}

const ColoredObject<NullInt>& DocItemView::GetGroup() const
{
	return m_group;
}
	
void DocItemView::SetCowNo(const CowNo& cowNo)
{
	m_cowNo = cowNo;
}

void DocItemView::SetPassNo(const std::wstring& passNo)
{
	m_passNo = passNo;
}

void DocItemView::SetFstOwnerAlias(const std::wstring& fstOwnerAlias)
{
	m_fstOwnerAlias = fstOwnerAlias;
}

void DocItemView::SetBirthDate(const DateTime& birthDt)
{
	m_birthDt = birthDt;
}

void DocItemView::SetWeight(const ColoredObject<Decimal>& weight)
{
	m_weight = weight;
}

void DocItemView::SetSex(const CowSex& sex)
{
	m_sex = sex;
}

void DocItemView::SetStock(const ColoredObject<std::wstring>& stockCd)
{
	m_stock = stockCd;
}

void DocItemView::SetClass(const ColoredObject<std::wstring>& classCd)
{
	m_class = classCd;
}

void DocItemView::SetGroup(const ColoredObject<NullInt>& group)
{
	m_group = group;
}

