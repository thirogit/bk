#ifndef __IUSER_H__
#define __IUSER_H__

#include <types/DateTime.h>
#include "IIdable.h"
#include "IFarm.h"

class IUser : public IIdable
{
public:
	virtual const std::wstring&  GetLogin() const = 0;
	virtual const std::wstring& GetFirstName() const = 0;
	virtual const std::wstring& GetLastName() const = 0;
	virtual const std::wstring& GetPassword() const = 0;
	virtual const DateTime& GetLastLogin() const = 0;
	virtual const IFarm* GetFarm() const = 0;
};




#endif

