#ifndef __IHERD_H__
#define __IHERD_H__

#include "types/HerdNo.h"
#include "IIdable.h"
#include <string>

class IHerd : public IIdable
{
public:
	virtual const std::wstring& GetHerdName() const = 0;
	virtual const std::wstring& GetCity() const = 0;
	virtual const std::wstring& GetZipCode() const = 0;
	virtual const std::wstring& GetStreet() const = 0;
	virtual const std::wstring& GetPOBox() const = 0;
	virtual int	               GetHerdIndex() const = 0;	
	
};

#endif
