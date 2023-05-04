#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include <data/impl/Hent.h>
#include <arrays/Enumerator.h>
#include "../wire/HentWireObject.h"
#include "../../CommonTypes.h"

class FetchHentsResponse
{
private:
	NewPtrFlushArray<HentWireObject> m_Hents;
public:
	FetchHentsResponse();
	~FetchHentsResponse();
	
	HentWireObject* AddHent(uint32_t hentId);
		
	PtrEnumerator<const HentWireObject> GetHents();
	count_t GetHentsCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchHentsResponse> FetchHentsResponseObj;
