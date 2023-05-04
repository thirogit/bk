#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 
#include "../PurchaseHeader.h"

class PurchaseHeaderSerializer
{
public:
	PurchaseHeaderSerializer(PurchaseHeader* purchaseHeader);
	void serialize(SF::Archive &archive);

private:
	std::wstring&		Id();
	std::wstring& AgentCode();
	std::wstring& PlateNo();
	DateTime& StartDate();
	DateTime& EndDate();
	bool&  WasDownloaded();
	int& HerdNumber();
	count_t& CowCount();

private:
	PurchaseHeader* m_header;
};
