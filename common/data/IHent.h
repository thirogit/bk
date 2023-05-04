#ifndef __IHENT_H__
#define __IHENT_H__

#include "IIdable.h"
#include "HentType.h"
#include <types/Latitude.h>
#include <types/Longitude.h>
#include "types/AccountNo.h"
#include "types/HentNo.h"
#include "types/DateTime.h"

class IHent : public IIdable
{
public:
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
	virtual bool GetSyncIt() const = 0;
	virtual const std::wstring& GetPESEL() const = 0;
	virtual const std::wstring& GetREGON() const = 0;
	virtual const std::wstring& GetIdNo() const = 0;
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
