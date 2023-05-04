#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include <arrays/Enumerator.h>
#include "../../configuration/print/PrintProfile.h"
#include "../../CommonTypes.h"

class FetchPrintProfilesResponse
{
private:
	NewPtrFlushArray<PrintProfile> m_Profiles;
public:
	FetchPrintProfilesResponse();
	~FetchPrintProfilesResponse();
	
	PrintProfile* AddProfile(uint32_t profileId);
		
	PtrEnumerator<IPrintProfile> GetProfiles();
	count_t GetProfilesCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchPrintProfilesResponse> FetchPrintProfilesResponseObj;
