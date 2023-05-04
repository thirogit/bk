#ifndef __PASSPORTRECOGNITIONPROFILEREQUEST_H__
#define __PASSPORTRECOGNITIONPROFILEREQUEST_H__

#include <boost/shared_ptr.hpp>
#include "../../configuration/passportrecognition/PassportRecognitionProfile.h"
#include <SF/Archive.hpp>

class PassportRecognitionProfileRequest
{
public:
	PassportRecognitionProfileRequest();
	~PassportRecognitionProfileRequest();

	PassportRecognitionProfile* GetProfile() const;
	PassportRecognitionProfile* CreateProfile(uint32_t profileId);

	void serialize(SF::Archive &archive);
	
private:
	PassportRecognitionProfile* m_pProfile;
};

typedef boost::shared_ptr<PassportRecognitionProfileRequest> PassportRecognitionProfileRequestObj;

#endif
