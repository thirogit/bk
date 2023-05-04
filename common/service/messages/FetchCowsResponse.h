#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include "../wire/CowWireObject.h"
#include <arrays/Enumerator.h>
#include "../../CommonTypes.h"

class FetchCowsResponse
{
private:
	NewPtrFlushArray<CowWireObject> m_Cows;
public:
	FetchCowsResponse();
	~FetchCowsResponse();
	
	CowWireObject* AddCow(uint32_t cowId);
		
	PtrEnumerator<const CowWireObject> GetCows();
	count_t GetCowsCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchCowsResponse> FetchCowsResponseObj;
