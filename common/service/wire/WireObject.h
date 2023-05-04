#pragma once 

#include <SF/Archive.hpp>

template<class OBJTYPE,class SERIALIZER>
class WireObject : public OBJTYPE
{
public: 
	WireObject(uint32_t id) : OBJTYPE(id),m_Locked(false)
	{
	}
	
	bool GetIsLocked() const
	{
		return m_Locked;
	}
	
	void SetIsLocked(bool bLocked)
	{
		m_Locked = bLocked;
	}	
	
	void serialize(SF::Archive &archive)
	{
		archive & m_Locked;
		SERIALIZER(this).serialize(archive);
	}
private:
	bool m_Locked;
};
