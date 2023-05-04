#ifndef __SINGLECONNECTIONPOOL_H__ 
#define __SINGLECONNECTIONPOOL_H__ 

template<class POOLTYPE>
class SingleConnectionPool : public POOLTYPE
{
protected:

	SingleConnectionPool()
	{
	}

	static SingleConnectionPool<POOLTYPE> m_instance;
public:
	static SingleConnectionPool* GetInstance()
	{
		return &m_instance;
	}
};

template<class POOLTYPE> SingleConnectionPool<POOLTYPE> SingleConnectionPool<POOLTYPE>::m_instance;


#endif