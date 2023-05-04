#include "stdafx.h"
#include "FetchClassesResponse.h"
#include <data/impl/serialization/SerializableClass.h>

FetchClassesResponse::FetchClassesResponse() 
{
}

FetchClassesResponse::~FetchClassesResponse()
{
		m_Classes.FreeAndFlush();
}	

Class* FetchClassesResponse::AddClass(uint32_t classId)
{
	Class* pNewClass = new Class(classId);
	m_Classes.Add(pNewClass);
	return pNewClass;
}
		
PtrEnumerator<IClass> FetchClassesResponse::GetClasses()
{
	return PtrEnumerator<IClass>(m_Classes);
}

count_t FetchClassesResponse::GetClassesCount() const
{
	return m_Classes.size();
}

void FetchClassesResponse::serialize(SF::Archive &archive)
{
	count_t classCount;

	if(archive.isRead())
	{
		archive & classCount;
		for (count_t i = 0; i < classCount; i++)
		{
			SerializableClass(AddClass(NULL_ID)).serialize(archive);
		}
	}	
	else
	{
		classCount = m_Classes.size();
		archive & classCount;
		for (count_t i = 0; i < classCount; i++)
		{
			SerializableClass(m_Classes[i]).serialize(archive);
		}
	}
}
