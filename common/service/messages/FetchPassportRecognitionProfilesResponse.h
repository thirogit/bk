#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include <arrays/NewPtrFlushArray.h>
#include <arrays/Enumerator.h>
#include "../../configuration/passportrecognition/PassportRecognitionProfile.h"
#include "../../CommonTypes.h"

class FetchPassportRecognitionProfilesResponse
{
private:
	NewPtrFlushArray<PassportRecognitionProfile> m_Profiles;
public:
	FetchPassportRecognitionProfilesResponse();
	~FetchPassportRecognitionProfilesResponse();
	
	PassportRecognitionProfile* AddProfile(uint32_t profileId);
		
	PtrEnumerator<IPassportRecognitionProfile> GetProfiles();
	count_t GetProfilesCount() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<FetchPassportRecognitionProfilesResponse> FetchPassportRecognitionProfilesResponseObj;
