#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../Stock.h"

class SerializableStock
{
public:
	SerializableStock(Stock* pStock);

	uint32_t&    Id();
	std::wstring& Name();
	std::wstring& Code();
	
	void serialize(SF::Archive &archive);

private:
	Stock* m_Stock;
};
