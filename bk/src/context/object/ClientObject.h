#ifndef __CLIENTOBJECT_H__
#define __CLIENTOBJECT_H__

template<class OBJECT>
class ClientObject
{
public:
	ClientObject(OBJECT* pObject) : m_pObject(pObject),m_Locked(false)
	{
	}
	
	~ClientObject()
	{
		delete m_pObject;
	}
	
	OBJECT* Object() const
	{
		return m_pObject;
	}
	
	bool IsLocked() const
	{
		return m_Locked;
	}
	
	void SetLocked(bool bLocked)
	{
		m_Locked = bLocked;
	}
private:	
	OBJECT* m_pObject;
	bool m_Locked;
};
#endif
