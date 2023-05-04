#ifndef __DOCVIEW_H__
#define __DOCVIEW_H__

#include <data\Motive.h>
#include <types\DateTime.h>
#include <types\Time.h>

struct DocViewMember
{
	enum 
	{
		DocNumber, 
		DocDate,
		LoadDate,
		PlateNo,
		CowCount,
		Extras,
		Agent,
		Motive,
		StartTime,
		EndTime,
		DocViewMember_LastMember			
	};
};

class DocView 
{
public:
	DocView(uint32_t docId);
	uint32_t				GetDocId() const;
	const std::wstring&		GetDocNo() const;
	const DateTime& 		GetDocDate() const;
	const DateTime& 		GetLoadDate() const;
	const std::wstring& 		GetExtras() const;
	const std::wstring& 		GetPlateNo() const;
	Motive 					GetMotive() const;	
	const Time& 			GetLoadStartDtTm() const;
	const Time& 			GetLoadEndDtTm() const;	
	const std::wstring&		GetAgentCd() const;
	size_t					GetCowCount() const;	
		
	void SetDocNo(const std::wstring& sDocNo);
	void SetDocDate(const DateTime& docDate);
	void SetLoadDate(const DateTime& loadDate);
	void SetExtras(const std::wstring& sExtras);
	void SetPlateNo(const std::wstring& sPlateNo);
	void SetMotive(Motive motive);	
	void SetLoadStartDtTm(const Time& loadStartTm);
	void SetLoadEndDtTm(const Time& loadEndTm);	
	void SetAgentCd(const std::wstring& agentCd);	
	void SetCowCount(size_t cowCount);	
private:
	uint32_t m_DocId;
	std::wstring m_DocNo;
	DateTime m_DocDate;
	DateTime m_LoadDate;
	std::wstring m_Extras;
	std::wstring m_PlateNo;
	Motive m_Motive;	
	Time m_LoadStartDtTm;
	Time m_LoadEndDtTm;	
	std::wstring m_AgentCd;	
	size_t m_CowCount;
	
};

#endif