#pragma once 

#include <data/impl/Cow.h>
#include <SF/Archive.hpp>

class CowWireObject : public Cow
{
public: 
	CowWireObject(uint32_t id);	
	bool GetIsLocked() const;
	void SetIsLocked(bool bLocked);	
	void serialize(SF::Archive &archive);
private:
	bool m_Locked;
};

