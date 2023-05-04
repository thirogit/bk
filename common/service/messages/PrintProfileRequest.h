#ifndef __PRINTPROFILEREQUEST_H__
#define __PRINTPROFILEREQUEST_H__

#include <boost/shared_ptr.hpp>
#include "../../configuration/print/PrintProfile.h"
#include <SF/Archive.hpp>

class PrintProfileRequest
{
public:
	PrintProfileRequest();
	~PrintProfileRequest();

	PrintProfile* GetProfile() const;	
	PrintProfile* CreateProfile(uint32_t profileId);

	void serialize(SF::Archive &archive);
	
private:
	PrintProfile* m_pProfile;
};

typedef boost::shared_ptr<PrintProfileRequest> PrintProfileRequestObj;

#endif
