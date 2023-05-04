#include "stdafx.h"
#include "SerializableOperationArguments.h"
#include <SF/any.hpp>
#include <SF/vector.hpp>

SerializableOperationArguments::SerializableOperationArguments(OperationArguments* pArguments) : m_pArguments(pArguments)
{
}

void SerializableOperationArguments::serialize(SF::Archive &archive)
{
	archive & m_pArguments->m_arguments;
}