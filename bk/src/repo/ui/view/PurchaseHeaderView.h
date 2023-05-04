#ifndef __PURCHASEVIEW_H__
#define __PURCHASEVIEW_H__

#include <string>
#include <stdint.h>
#include <types\DateTime.h>

struct PurchaseHeaderViewMember
{
	enum
	{		
		PurchaseHeaderViewMember_Herd,
		PurchaseHeaderViewMember_Agent,
		PurchaseHeaderViewMember_StartDt,
		PurchaseHeaderViewMember_CowAmount,
		PurchaseHeaderViewMember_PlateNo,
		PurchaseHeaderViewMember_Downloaded,		
	};
};


class PurchaseHeaderView 
{
public:
	PurchaseHeaderView(const std::wstring& purchaseId);
	const std::wstring& GetPurchaseId() const;
	
	int GetHerd() const;
	const std::wstring& GetAgent() const;
	const DateTime& GetStartDt() const;
	int GetCowAmount() const;
	bool GetDownloaded() const;
	const std::wstring& GetPlateNo() const;
	
	void SetHerd(int herdIndex);
	void SetAgent(const std::wstring&);
	void SetStartDt(const DateTime& );
	void SetCowAmount(int amount);
	void SetDownloaded(bool downloaded);
	void SetPlateNo(const std::wstring& plateNo);
	
private:
	std::wstring m_purchaseId;
	int m_Herd;
	std::wstring m_Agent;
	DateTime m_StartDt;
	int m_CowAmount;
	bool m_Downloaded;
	std::wstring m_PlateNo;
};

#endif