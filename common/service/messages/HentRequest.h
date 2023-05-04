#ifndef __HENTREQUEST_H__
#define __HENTREQUEST_H__

#include <boost/shared_ptr.hpp>
#include <data/impl/Hent.h>
#include <SF/Archive.hpp>

class HentRequest
{
public:
	HentRequest();
	~HentRequest();

	IHent* GetHent() const;	
	Hent* CreateHent(uint32_t hentId);

	void serialize(SF::Archive &archive);
	
private:
	Hent* m_pHent;	
};

typedef boost::shared_ptr<HentRequest> HentRequestObj;

#endif
