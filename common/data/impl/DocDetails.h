#ifndef __DOCDETAILS_H__
#define __DOCDETAILS_H__


#include "../../types/DateTime.h"
#include <string>
#include "../Motive.h"
#include "../../types/Time.h"
#include "../IDoc.h"

class SerializableDocDetails;

class DocDetails 
{
public:
	DocDetails();

	void CopyFrom(const DocDetails& src);
	void CopyFrom(const IDoc* pSrc);

	const DateTime& 		GetDocDate() const;
	const DateTime& 		GetLoadDate() const;
	const std::wstring& 	GetExtras() const;
	const std::wstring& 	GetPlateNo() const;
	Motive 					GetMotive() const;	
	const Time& 			GetLoadStartDtTm() const;
	const Time& 			GetLoadEndDtTm() const;	
	uint32_t 				GetAgentId() const;

	void SetDocDate(const DateTime& docDate);
	void SetLoadDate(const DateTime& loadDate);
	void SetExtras(const std::wstring& sExtras);
	void SetPlateNo(const std::wstring& sPlateNo);
	void SetMotive(Motive motive);	
	void SetLoadStartDtTm(const Time& loadStartDtTm);
	void SetLoadEndDtTm(const Time& loadEndDtTm);	
	void SetAgentId(uint32_t agentId);
	
protected:
	DateTime m_DocDate;
	DateTime m_LoadDate;
	std::wstring m_Extras;
	std::wstring m_PlateNo;
	Motive 	m_Motive;	
	Time m_LoadStartDtTm;
	Time m_LoadEndDtTm;	
	uint32_t m_AgentId;	

	friend class SerializableDocDetails;
};

#endif