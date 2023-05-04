#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../DocDetails.h"

class SerializableDocDetails
{
public:
	SerializableDocDetails(DocDetails* pDocDetails);	
	void serialize(SF::Archive &archive);
protected:
	
	DateTime& DocDate();
	std::wstring& Extras();
	std::wstring& PlateNo();
	Motive& MotiveOfDisplacement();	
	Time& LoadStartDtTm();
	Time& LoadEndDtTm();	
	uint32_t& AgentId();
	DateTime& LoadDate();

private:
	DocDetails* m_pDocDetails;
};
