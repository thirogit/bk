#include "stdafx.h"
#include "RecognitionItemView.h"


RecognitionItemView::RecognitionItemView(ItemId id)
{
	m_Id = id;
}

ItemId RecognitionItemView::GetId() const
{
	return m_Id;
}

const std::wstring& RecognitionItemView::GetClassCd() const
{
	return m_classCd;
}

void RecognitionItemView::SetClassCd(const std::wstring& classCd)
{
	m_classCd = classCd;
}

const Decimal& RecognitionItemView::GetWeight() const
{
	return m_weight;
}
void RecognitionItemView::SetWeight(const Decimal& weight)
{
	m_weight = weight;
}

const std::wstring& RecognitionItemView::GetStockCd()  const
{
	return m_stockCd;
}

void RecognitionItemView::SetStockCd(const std::wstring& stockCd)
{
	m_stockCd = stockCd;
}

const CowNo& RecognitionItemView::GetCowNo() const
{
	return m_cowno;
}

void RecognitionItemView::SetCowNo(const CowNo& cowNo)
{
	m_cowno = cowNo;
}

const CowNo& RecognitionItemView::GetMotherNo() const
{
	return m_motherNo;
}

void RecognitionItemView::SetMotherNo(const CowNo& motherNo)
{
	m_motherNo = motherNo;
}

const DateTime& RecognitionItemView::GetBirthDate() const
{
	return m_birthdate;
}

void RecognitionItemView::SetBirthDate(const DateTime& birthDt)
{
	m_birthdate = birthDt;
}

const std::wstring& RecognitionItemView::GetPassNo() const
{
	return m_passno;
}
void RecognitionItemView::SetPassNo(const std::wstring& passNo)
{
	m_passno = passNo;
}

const DateTime& RecognitionItemView::GetPassDate() const
{
	return m_passdate;
}

void RecognitionItemView::SetPassDate(const DateTime& passDt)
{
	m_passdate = passDt;
}

const std::wstring& RecognitionItemView::GetBirthPlace() const
{
	return m_birthplace;
}

void RecognitionItemView::SetBirthPlace(const std::wstring& birthPlace)
{
	m_birthplace = birthPlace;
}


const std::wstring& RecognitionItemView::GetFirstOwnerAlias() const
{
	return m_fstownerAlias;
}

void RecognitionItemView::SetFirstOwnerAlias(const std::wstring& hentAlias)
{
	m_fstownerAlias = hentAlias;
}

const CowSex& RecognitionItemView::GetSex() const
{
	return m_sex;
}

void RecognitionItemView::SetSex(const CowSex& cowsex)
{
	m_sex = cowsex;
}



const std::wstring& RecognitionItemView::GetExtras() const
{
	return m_extras;
}

void RecognitionItemView::SetExtras(const std::wstring& sExtras)
{
	m_extras = sExtras;
}
