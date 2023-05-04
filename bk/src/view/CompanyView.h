#ifndef __COMPANYVIEW_H__
#define __COMPANYVIEW_H__

#include <string>
#include <data\IFarm.h>
#include <types\Longitude.h>
#include <types\Latitude.h>

struct CompanyViewMember
{
	enum
	{
		Name,
		FarmNo,
		Street,
		POBox,
		City,
		Zip,		
		PhoneNo,
		FiscalNo,
		WetNo,
		StatNo,
		WetLicenceNo,
		CellPhoneNo,
		EmailAddress,
		Latitude,
		Longitude
	};
};

class CompanyView 
{
public:
	CompanyView();
	CompanyView(const CompanyView& src);
	void CopyFrom(const CompanyView& src);
	void CopyFrom(const IFarm* src);	
		
	const std::wstring& GetName() const;
	void SetName(const std::wstring& sName);
	
	const std::wstring& GetStreet() const;
	void SetStreet(const std::wstring& sStreet);
		
	const std::wstring& GetPOBox() const;
	void SetPOBox(const std::wstring& sPOBox);
	
	const std::wstring& GetCity() const;
	void SetCity(const std::wstring& sCity);
	
	const std::wstring& GetZip() const;
	void SetZip(const std::wstring& sZip);
	
	const std::wstring& GetPhoneNo() const;
	void SetPhoneNo(const std::wstring& sPhoneNo);
	
	const std::wstring& GetFiscalNo() const;
	void SetFiscalNo(const std::wstring& sFiscalNo);
	
	const FarmNo& GetFarmNo() const;
	void SetFarmNo(const FarmNo& farmNo);
	
	const std::wstring& GetWetNo() const;
	void SetWetNo(const std::wstring& sWetNo);	
	
	const std::wstring& GetREGON() const;
	void SetREGON(const std::wstring& sREGON);
	
	const std::wstring& GetWetLicenceNo() const;
	void SetWetLicenceNo(const std::wstring& sWetLicenceNo);
	
	const std::wstring& GetCellPhoneNo() const;
	void SetCellPhoneNo(const std::wstring& sCellPhoneNo);

	const std::wstring& GetEmailAddress() const;
	void SetEmailAddress(const std::wstring& sEmailAddress);

	const Latitude& GetLatitude() const;
	void SetLatitude(const Latitude& lati);

	const Longitude& GetLongitude() const;
	void SetLongitude(const Longitude& longi);
	
protected:
	std::wstring m_name;
	std::wstring m_street;
	std::wstring m_pobox;
	std::wstring m_city;
	std::wstring m_zip;
	std::wstring m_phone;
	std::wstring m_nip;
	FarmNo m_farmno;
	std::wstring m_wetno;
	std::wstring m_regon;
	std::wstring m_wetlicenceno;
	std::wstring m_cellphoneno;
	std::wstring m_emailaddress;
	Latitude m_latitude;
	Longitude m_longitude;
};

#endif