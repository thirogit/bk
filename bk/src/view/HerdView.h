#ifndef __HERDVIEW_H__
#define __HERDVIEW_H__

#include <string>

struct HerdViewMember
{
	enum
	{
		Name,
		HentNo,
		Street,
		POBox,
		City,
		Zip,
		Index
	};
};

class HerdView 
{
public:
	HerdView(uint32_t Id);
	HerdView(const HerdView& src);
	void CopyFrom(const HerdView& src);
	HerdView& operator=(const HerdView& src);
			
	uint32_t GetHerdId() const;
		
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
	
	
protected:
	uint32_t m_Id;
	std::wstring m_name;
	std::wstring m_street;
	std::wstring m_pobox;
	std::wstring m_city;
	std::wstring m_zip;
	
};

#endif