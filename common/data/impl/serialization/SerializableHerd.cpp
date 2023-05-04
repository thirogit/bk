#include "stdafx.h"
#include "SerializableHerd.h"
#include "../../types/serialization/TypesSerialization.h"

SerializableHerd::SerializableHerd(Herd* pHerd) : m_pHerd(pHerd)
{

}
	
std::wstring& SerializableHerd::HerdName()
{
	return m_pHerd->m_HerdName;
}

std::wstring& SerializableHerd::City()
{
	return m_pHerd->m_City;
}

std::wstring& SerializableHerd::ZipCode()
{
	return m_pHerd->m_ZipCode;
}

std::wstring& SerializableHerd::Street()
{
	return m_pHerd->m_Street;
}

std::wstring& SerializableHerd::POBox()
{
	return m_pHerd->m_POBox;
}

int& SerializableHerd::HerdIndex()
{
	return m_pHerd->m_HerdIndex;
}

uint32_t& SerializableHerd::Id()
{
	return m_pHerd->m_Id;
}
	
void SerializableHerd::serialize(SF::Archive &archive)
{
	archive & Id()
			& HerdName()
			& City()
			& ZipCode()
			& Street()
			& POBox()	
			& HerdIndex();	
}