#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include <data/impl/Class.h>
#include <arrays/Enumerator.h>
#include "../../CommonTypes.h"

class FetchClassesResponse
{
private:
	NewPtrFlushArray<Class> m_Classes;
public:
	FetchClassesResponse();
	~FetchClassesResponse();
	
	Class* AddClass(uint32_t classId);
		
	PtrEnumerator<IClass> GetClasses();
	count_t GetClassesCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchClassesResponse> FetchClassesResponseObj;
