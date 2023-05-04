#pragma once

#include "LockableObject.h"

template<class OBJ>
class ServerObject : public LockableObject
{
public:
	typedef OBJ object_type;

	ServerObject(OBJ* pObject) : m_pObject(pObject)
	{
	}
	~ServerObject()
	{
		delete m_pObject;
	}

	OBJ* Object()
	{
		return m_pObject;
	}

private:
	OBJ* m_pObject;

};

