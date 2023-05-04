#ifndef __FARM_H__
#define __FARM_H__

#include "../IFarm.h"

class SerialazibleFarm;

class Farm : public IFarm
{
public:
	Farm();
	Farm(uint32_t Id);
	Farm(const IFarm* src);
	virtual const std::wstring& GetName() const;
	virtual const std::wstring& GetAlias() const;
	virtual const std::wstring& GetCity() const;
	virtual const std::wstring& GetZipCode() const;
	virtual const std::wstring& GetStreet() const;
	virtual const std::wstring& GetPOBox() const;
	virtual const FarmNo& GetFarmNo() const;	
	virtual const std::wstring& GetNIP() const;
	virtual const std::wstring& GetREGON() const;
	virtual const std::wstring& GetWetIdNo() const;
	virtual const std::wstring& GetWetLicNo() const;
	virtual const std::wstring& GetPhoneNo() const;
	virtual uint32_t GetId() const;	

	virtual void SetName(const std::wstring&);
	virtual void SetAlias(const std::wstring&);
	virtual void SetCity(const std::wstring&);
	virtual void SetZipCode(const std::wstring&);
	virtual void SetStreet(const std::wstring&);
	virtual void SetPOBox(const std::wstring&);
	virtual void SetFarmNo(const FarmNo&);	
	virtual void SetNIP(const std::wstring&);
	virtual void SetREGON(const std::wstring&);
	virtual void SetWetIdNo(const std::wstring&);
	virtual void SetWetLicNo(const std::wstring&);
	virtual void SetPhoneNo(const std::wstring&);

	void CopyFrom(const IFarm* pSrc);
protected:	
	
	std::wstring m_Name;
	std::wstring m_Alias;
	std::wstring m_City;
	std::wstring m_ZipCode;
	std::wstring m_Street;
	std::wstring m_POBox;
	FarmNo      m_FarmNo;	
	std::wstring m_NIP;
	std::wstring m_REGON;
	std::wstring m_WetIdNo;
	std::wstring m_WetLicNo;
	std::wstring m_PhoneNo;	
	uint32_t    m_Id;
	
	friend class SerializableFarm;
};




#endif