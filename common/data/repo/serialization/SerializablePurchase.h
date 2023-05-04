#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 
#include "../Purchase.h"

class SerializablePurchase
{
public:
	SerializablePurchase(Purchase* purchase);
	void serialize(SF::Archive &archive);

private:
	std::wstring&		Id();
	std::wstring& AgentCode();
	std::wstring& PlateNo();
	DateTime& StartDate();
	DateTime& EndDate();
	bool&  WasDownloaded();
	int& HerdNumber();

private:
	Purchase* m_purchase;
};
