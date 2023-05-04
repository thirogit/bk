#ifndef __IFARM_H__
#define __IFARM_H__

#include "types/FarmNo.h"
#include "types/AccountNo.h"
#include "IIdable.h"

class IFarm : public IIdable
{
public:
	virtual const std::wstring& GetName() const = 0;
	virtual const std::wstring& GetAlias() const = 0;
	virtual const std::wstring& GetCity() const = 0;
	virtual const std::wstring& GetZipCode() const = 0;
	virtual const std::wstring& GetStreet() const = 0;
	virtual const std::wstring& GetPOBox() const = 0;
	virtual const FarmNo&	   GetFarmNo() const = 0;	
	virtual const std::wstring& GetNIP() const = 0;
	virtual const std::wstring& GetREGON() const = 0;
	virtual const std::wstring& GetWetIdNo() const = 0;
	virtual const std::wstring& GetWetLicNo() const = 0;
	virtual const std::wstring& GetPhoneNo() const = 0;
};

#endif