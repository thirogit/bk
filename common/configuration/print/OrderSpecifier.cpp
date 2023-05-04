#include "stdafx.h"
#include "OrderSpecifier.h"

OrderSpecifier::OrderSpecifier(const OrderSpecifier& src)
{
	m_FieldCode = src.m_FieldCode;
	m_Asc = src.m_Asc;
}

OrderSpecifier::OrderSpecifier(const std::wstring& sFieldCode)
{
	m_FieldCode = sFieldCode;
	m_Asc = false;
}

const std::wstring& OrderSpecifier::fieldCode() const
{
	return m_FieldCode;
}

bool OrderSpecifier::asc() const
{
	return m_Asc;
}

void OrderSpecifier::asc(bool bAsc)
{
	m_Asc = bAsc;
}
