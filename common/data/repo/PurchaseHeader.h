#ifndef __PURCHASEHEADER_H__
#define __PURCHASEHEADER_H__

#include "IPurchaseHeader.h"
#include "IPurchase.h"

class PurchaseHeaderSerializer;

class PurchaseHeader : public IPurchaseHeader
{
public:	
	PurchaseHeader(const std::wstring& id);
	PurchaseHeader(const PurchaseHeader& src);
	PurchaseHeader(const IPurchase* src);
	PurchaseHeader(const IPurchaseHeader* src);
	~PurchaseHeader();
	void CopyFrom(const PurchaseHeader& src);
	void CopyFrom(const IPurchase* src);
	void CopyFrom(const IPurchaseHeader* src);
	PurchaseHeader& operator=(const PurchaseHeader& right);

	virtual const std::wstring& GetId() const;
	virtual int GetHerd() const;
	virtual const std::wstring& GetAgentCode() const;	
	virtual const std::wstring& GetPlateNo() const;
	virtual const DateTime& GetStartDate() const;
	virtual const DateTime& GetEndDate() const;
	virtual bool  GetWasDownloaded() const;	
	virtual count_t GetCowCount() const;
			
	void SetHerd(int herd);
	void SetAgentCode(const std::wstring& sAgentCode);
	void SetPlateNo(const std::wstring& sPlateNo);
	void SetStartDate(const DateTime& startDt);
	void SetEndDate(const DateTime& endDt);
	void SetWasDownloaded(bool bDownloaded);
	void SetCowCount(count_t cowCount);	
	
	friend class PurchaseHeaderSerializer;

private:
	int m_Herd;
	std::wstring m_AgentCode;
	std::wstring m_PlateNo;
	DateTime m_StartDate;
	DateTime m_EndDate;
	std::wstring m_PuracheId;
	bool m_Downloaded;
	count_t m_CowCount;

};


#endif