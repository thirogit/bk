#ifndef __SERVICEREGISTRY_H__
#define __SERVICEREGISTRY_H__

#include <typeinfo>

class AppService;

class ServiceRegistry
{
public:
	template<typename Type> Type * FindByType(Type *)
	{
		return (Type*)FindByType(typeid(Type));
	}

	virtual AppService* FindByType(const std::type_info &ti) = 0;	
};

#endif