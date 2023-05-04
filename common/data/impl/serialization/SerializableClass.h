#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../Class.h"

class SerializableClass
{
public:
	SerializableClass(Class* pClass);

	uint32_t&		Id();
	std::wstring& 	ClassName();	
	std::wstring& 	ClassCode();	
	NullDecimal&	MyPerKgPrice();	
	CowSex&			PredefSex();	
	uint32_t&		RuleId();
	
	void serialize(SF::Archive &archive);

private:
	Class* m_pClass;
};
