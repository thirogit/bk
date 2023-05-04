#ifndef __HENTNO_H__
#define __HENTNO_H__

#include "HerdNo.h"
#include "FarmNo.h"
#include "../../types/Nullable.h"
#include <boost/optional.hpp>
#include "../../exception/TagException.h"

struct tag_HentNoException{};
typedef TagException<tag_HentNoException> HentNoException;

class HentNo;

class HentNoFactory
{
public:
	static HentNo Create(const FarmNo& farmNo,const HerdNo& herdNo);	
	static HentNo Create(const Country& country,const std::wstring& number);
	static HentNo FromString(const std::wstring& sHentNo);
};

class HentNo : public Nullable
{
	class InternalHentNo
	{
	public:
		HerdNo m_HerdNo;
		FarmNo m_FarmNo;
		bool operator==(const InternalHentNo& right) const;
	};

	HentNo(const FarmNo& farmNo,const HerdNo& herdNo);
public:
	HentNo();
	HentNo(const HentNo& hentNo);
	std::wstring ToString() const;
	HentNo& operator=(const HentNo& right);
	Country GetCountry() const;
	std::wstring GetNumber() const;
	FarmNo GetFarmNo() const;
	HerdNo GetHerdNo() const;
	void CopyFrom(const HentNo& src);
	virtual void MakeNull();
	virtual bool IsNull() const;

	
protected:
	boost::optional<InternalHentNo> m_Value;
#if _DEBUG
	std::wstring m_stringValue;
#endif
	friend class HentNoFactory;	

	friend bool operator>=(const HentNo& left, const HentNo& right);
	friend bool operator<=(const HentNo& left, const HentNo& right);
	friend bool operator>(const HentNo& left, const HentNo& right);
	friend bool operator<(const HentNo& left, const HentNo& right);
	friend bool operator==(const HentNo& left, const HentNo& right);
	friend bool operator!=(const HentNo& left, const HentNo& right);

};


#endif
