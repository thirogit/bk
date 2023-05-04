#ifndef __COWREQUEST_H__
#define __COWREQUEST_H__

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include "../../data/impl/CowDetails.h"

class CowRequest
{
public:
	CowRequest(uint32_t cowId = NULL_ID);
	uint32_t GetCowId() const;
	CowRequest& WithDetails(const CowDetails& details);	
	const CowDetails& GetDetails() const;
	void serialize(SF::Archive &archive);	
private:
	CowDetails m_details;
	uint32_t m_cowId;
};

typedef boost::shared_ptr<CowRequest> CowRequestObj;

#endif
