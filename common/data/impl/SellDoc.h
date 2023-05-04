#ifndef __SELLDOC_H__
#define __SELLDOC_H__

#include "../ISellDoc.h"
#include "DocContent.h"
#include "DocDetails.h"

class SerializableSellDoc;

class SellDoc : public ISellDoc
{
public:
	typedef ISellDoc interface_type;

	SellDoc(uint32_t docId);
	virtual void 					CopyFrom(ISellDoc* pSrc);
	virtual uint32_t				GetId() const;	
	virtual const DateTime& 		GetDocDate() const;
	virtual const DateTime& 		GetLoadDate() const;
	virtual const std::wstring& 	GetExtras() const;
	virtual const std::wstring& 	GetPlateNo() const;
	virtual Motive 					GetMotive() const;	
	virtual const Time& 			GetLoadStartDtTm() const;
	virtual const Time& 			GetLoadEndDtTm() const;	
	virtual uint32_t 				GetAgentId() const;
	virtual int 					GetCowCount() const;	
	virtual PtrEnumerator<ICowEntry> EnumCows() const;
	virtual ICowEntry*				GetCow(uint32_t cowId) const;	
	virtual ICowEntry*				GetCowAt(int index) const;
	virtual uint32_t				GetHerdId() const;
	virtual uint32_t 				GetHentId() const;
	virtual uint32_t				GetInvoiceId() const;
	virtual DocType					GetDocType() const;
	
	void							SetDetails(const DocDetails& details);
	void							SetDetails(IDoc* pSrc);

	void 							SetDocDate(const DateTime& docDate);
	void 							SetLoadDate(const DateTime& loadDate);
	void 							SetExtras(const std::wstring& sExtras);
	void 							SetPlateNo(const std::wstring& sPlateNo);
	void 							SetMotive(Motive motive);	
	void 							SetLoadStartDtTm(const Time& loadStartTm);
	void 							SetLoadEndDtTm(const Time& loadEndTm);	
	void 							SetAgentId(uint32_t agentId);	
	void 							RemoveAt(int index);
	void 							RemoveAll();
	CowEntry* 						AddCow(uint32_t cowId);
	void							AddCow(CowEntry* pCow);
	void							SetHerdId(uint32_t herdId);
	void  							SetHentId(uint32_t hentId);
	void							SetInvoiceId(uint32_t invoiceId);
	void							RemoveEntry(uint32_t cowId);
	CowEntry*						GetEntry(uint32_t cowId);


private:
	uint32_t m_DocId;
	uint32_t m_hentId;
	uint32_t m_invoiceId;
	uint32_t m_herdId;
	DocDetails m_Details;
	DocContent m_Content;

	friend class SerializableSellDoc;
};

#endif