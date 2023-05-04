#ifndef __DEDUCTIONDETAILS_H__
#define __DEDUCTIONDETAILS_H__

#include <string>
#include <types/Decimal.h>
#include  <casajson/json.h>

class DeductionDetails 
{
public:	
	DeductionDetails();
	DeductionDetails(const DeductionDetails& src);
	DeductionDetails& operator=(const DeductionDetails& src);

	void CopyFrom(const DeductionDetails& src);
	
	const std::wstring& 	GetCode() const;	
	const std::wstring& 	GetReason() const;
	const Decimal&	GetFraction() const;
	   	
	void SetCode(const std::wstring& sCode);
	void SetReason(const std::wstring& sReason);
	void SetFraction(const Decimal&	fraction);

	static DeductionDetails FromJson(const json::value& value);
	json::value ToJson() const;
private:
	std::wstring m_code;
	std::wstring m_reason;
	Decimal	m_fraction;

};


#endif
