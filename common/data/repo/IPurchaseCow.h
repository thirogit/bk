#ifndef __IPURCHASECOW_H__
#define __IPURCHASECOW_H__

#include "../types/CowNo.h"
#include "../types/CowSex.h"
#include "../../types/Decimal.h"
#include "../types/HentNo.h"
#include "../../types/Longitude.h"
#include "../../types/Latitude.h"
#include "../../types/DateTime.h"
#include "../IIdable.h"

class IPurchaseCow 
{
public:	
	virtual const std::wstring& GetId() const = 0;
	virtual const CowNo& GetCowNo() const = 0;
	virtual const CowSex& GetSex() const = 0;	
	virtual const std::wstring& GetStockCd() const = 0;
	virtual const std::wstring& GetClassCd() const = 0;
	virtual const Decimal& GetWeight() const = 0;	
	virtual const Decimal& GetPrice() const = 0;	
	virtual const Latitude& GetLatitude() const = 0;
	virtual const Longitude& GetLongitude() const = 0;
	virtual const std::wstring& GetPassportNo() const = 0;	
	virtual const HentNo& GetFirstOwner() const = 0;
	virtual const DateTime& GetPassportIssueDt() const = 0;
	virtual const std::wstring& GetHealthCertNo() const = 0;
	virtual const CowNo& GetMotherNo() const = 0;
	virtual const std::wstring& GetBirthPlace() const = 0;
	virtual const DateTime& GetBirthDt() const = 0;
	
};
	

#endif
