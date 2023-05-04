#ifndef __IPURCHASEHEADER_H__
#define __IPURCHASEHEADER_H__

#include <string>
#include <types/DateTime.h>
#include <CommonTypes.h>

class IPurchaseHeader
{
public:	
	virtual const std::wstring& GetId() const = 0;
	virtual int GetHerd() const = 0;
	virtual const std::wstring& GetAgentCode() const = 0;	
	virtual const std::wstring& GetPlateNo() const = 0;
	virtual const DateTime& GetStartDate() const = 0;
	virtual const DateTime& GetEndDate() const = 0;
	virtual bool  GetWasDownloaded() const = 0;	
	virtual count_t GetCowCount() const = 0;		

};


#endif
