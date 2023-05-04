#ifndef __HERD_H__
#define __HERD_H__

#include "../IHerd.h"
#include <string>
#include "../types/HerdNo.h"

class SerializableHerd;

class Herd : public IHerd
{
public:	
	Herd(uint32_t id);
	Herd(const IHerd* pSrc);
	virtual const std::wstring& GetHerdName() const;
	virtual const std::wstring& GetCity() const;
	virtual const std::wstring& GetZipCode() const;
	virtual const std::wstring& GetStreet() const;
	virtual const std::wstring& GetPOBox() const;
	virtual int                GetHerdIndex() const;
	virtual uint32_t		   GetId() const;

	void SetHerdName(const std::wstring&);
	void SetCity(const std::wstring&);
	void SetZipCode(const std::wstring&);
	void SetStreet(const std::wstring&);
	void SetPOBox(const std::wstring&);
	void SetHerdIndex(int);	

	void CopyFrom(const IHerd* pSrc);
	
private:	
	
	std::wstring m_HerdName;
	std::wstring m_City;
	std::wstring m_ZipCode;
	std::wstring m_Street;
	std::wstring m_POBox;
	uint32_t	m_Id;
	int			m_HerdIndex;
	
	friend SerializableHerd;
};




#endif