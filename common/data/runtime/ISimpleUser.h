#ifndef __ISIMPLEUSER_H__
#define __ISIMPLEUSER_H__

#include "../IIdable.h"
#include <string>

class ISimpleUser : public IIdable
{
public:
	virtual const std::wstring& GetLogin() const = 0;
	virtual const std::wstring& GetFirstName() const = 0;
	virtual const std::wstring& GetLastName() const = 0;
};

#endif