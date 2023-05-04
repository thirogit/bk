#pragma once 

#include <data/impl/Hent.h>
#include <SF/Archive.hpp>

class HentWireObject : public Hent
{
public: 
	HentWireObject(uint32_t id);	
	bool GetIsLocked() const;
	void SetIsLocked(bool bLocked);	
	void serialize(SF::Archive &archive);
private:
	bool m_Locked;
};

