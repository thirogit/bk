#include "stdafx.h"
#include "SerializableStock.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableStock::SerializableStock(Stock* pStock) : m_Stock(pStock)
{
}

std::wstring& SerializableStock::Name()
{
	return m_Stock->m_stockname;
}

std::wstring& SerializableStock::Code()
{
	return m_Stock->m_stockcode;
}

uint32_t& SerializableStock::Id()
{
	return m_Stock->m_stockid;
}

void SerializableStock::serialize(SF::Archive &archive)
{
	archive & Id();
	archive & Name();
	archive & Code();
}