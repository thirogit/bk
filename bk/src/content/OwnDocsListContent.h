#pragma once
#include "..\ctrls\contentlistctrl\ListContent.h"
#include "../context/SeasonSession.h"
#include "../context/ForwardHerdObserver.h"
#include "OwnDocRow.h"
#include "DocRowsIndex.h"

class OwnDocsListContent : public ListContent,public ForwardHerdObserver
{	
	typedef DocRowsIndex<OwnDocRow> OwnDocRowIndex;	

public:
	OwnDocsListContent();
	~OwnDocsListContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	OwnDocRow* GetDocAtRow(int row);
	uint32_t GetHerdId() const;
	void Create(SeasonSession* pSession,uint32_t herdId);
	void Destroy();
protected:
	void AddRow(OwnDocRow* row);
	virtual void CreateContent() = 0;
	OwnDocRowIndex m_Rows;
	SeasonSession* m_pSession;
	uint32_t m_herdId;
};


