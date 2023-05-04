#ifndef __MOVEDOC_H__
#define __MOVEDOC_H__

#include "../IMoveDoc.h"
#include "DocContent.h"
#include "DocDetails.h"

class SerializableMoveDoc;

class MoveDoc : public IMoveDoc
{
public:
	typedef IMoveDoc interface_type;
	
	MoveDoc(uint32_t docId);
	virtual void 					CopyFrom(IMoveDoc* pSrc);
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
	virtual uint32_t 				GetSrcHerdId() const;
	virtual uint32_t				GetDstHerdId() const;
	virtual DocType					GetDocType() const;
			const DocDetails&		GetDetails() const;

	
	void							SetDetails(const DocDetails& details);
	void							SetDetails(IDoc* pSrc);

	void 							SetDocDate(const DateTime& docDate);
	void 							SetLoadDate(const DateTime& loadDate);
	void 							SetExtras(const std::wstring& sExtras);
	void 							SetPlateNo(const std::wstring& sPlateNo);
	void 							SetLoadStartDtTm(const Time& loadStartTm);
	void 							SetLoadEndDtTm(const Time& loadEndTm);	
	void 							SetAgentId(uint32_t agentId);	
	void 							RemoveAt(int index);
	void 							RemoveAll();
	CowEntry* 						AddCow(uint32_t cowId);
	void							AddCow(CowEntry* pCow);
	void  							SetSrcHerdId(uint32_t herdFromId);
	void							SetDstHerdId(uint32_t herdtoId);
	void							RemoveEntry(uint32_t cowId);
	CowEntry*						GetEntry(uint32_t cowId);
private:
	uint32_t m_herdFromId;
	uint32_t m_herdToId;
	uint32_t m_DocId;
	DocDetails m_Details;
	DocContent m_Content;

	friend class SerializableMoveDoc;


};

#endif