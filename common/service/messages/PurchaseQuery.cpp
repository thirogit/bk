#include "stdafx.h"
#include "PurchaseQuery.h"
#include <data/types/serialization/TypesSerialization.h>
#include <SF/string.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


PurchaseQuery::PurchaseQuery()
{

}

PurchaseQuery& PurchaseQuery::WithStartDt(const DateTime& startDt)
{
	m_startDt = startDt;
	return *this;
}

PurchaseQuery& PurchaseQuery::WithEndDt(const DateTime& endDt)
{
	m_endDt = endDt;
	return *this;
}

const DateTime& PurchaseQuery::GetStartDt() const
{
	return m_startDt;
}

const DateTime& PurchaseQuery::GetEndDt() const
{
	return m_endDt;
}

void PurchaseQuery::serialize(SF::Archive &archive)
{
	archive & m_startDt & m_endDt;
}
