#ifndef __IPURCHASEHENT_H__
#define __IPURCHASEHENT_H__

#include <string>
#include "../HentType.h"
#include "../types/HentNo.h"
#include "../types/AccountNo.h"
#include "../../types/DateTime.h"
#include "../../types/Longitude.h"
#include "../../types/Latitude.h"
#include "../IIdable.h"

class IPurchaseHent 
{
public:
	virtual const std::wstring& GetId() const = 0;
	virtual const std::wstring& GetName() const = 0;
	virtual const std::wstring& GetAlias() const = 0;
	virtual const std::wstring& GetStreet() const = 0;
	virtual const std::wstring& GetPOBox() const = 0;
	virtual const std::wstring& GetCity() const = 0;
	virtual const std::wstring& GetZip() const = 0;
	virtual const std::wstring& GetPhoneNo() const = 0;
	virtual const std::wstring& GetFiscalNo() const = 0;
	virtual const HentNo& GetHentNo() const = 0;
	virtual const std::wstring& GetWetNo() const = 0;
	virtual const std::wstring& GetExtras() const = 0;
	virtual const std::wstring& GetPlateNo() const = 0;	
	virtual HentType GetHentType() const = 0;
	virtual const std::wstring& GetPersonalNo() const = 0;
	virtual const std::wstring& GetStatsNo() const = 0;
	virtual const std::wstring& GetPersonalIdNo() const = 0;
	virtual const DateTime& GetIssueDate() const = 0;
	virtual const std::wstring& GetIssuePost() const = 0;
	virtual const AccountNo& GetAccountNo() const = 0;
	virtual const std::wstring& GetBankName() const = 0;
	virtual const std::wstring& GetWetLicenceNo() const = 0;
	virtual const std::wstring& GetCellPhoneNo() const = 0;
	virtual const std::wstring& GetEmailAddress() const = 0;
	virtual const Latitude& GetLatitude() const = 0;
	virtual const Longitude& GetLongitude() const = 0;
};
	

#endif