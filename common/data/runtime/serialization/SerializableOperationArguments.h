#pragma once

#include <SF/Archive.hpp>
#include "../../../data/runtime/OperationArguments.h"

class SerializableOperationArguments
{
public:
	SerializableOperationArguments(OperationArguments* pArguments);
	void serialize(SF::Archive &archive);	
private:
	OperationArguments* m_pArguments;
};
