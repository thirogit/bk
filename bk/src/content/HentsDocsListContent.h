#pragma once
#include "..\ctrls\contentlistctrl\ListContent.h"
#include "../context/SeasonSession.h"
#include "../context/ForwardHerdObserver.h"
#include "HentsDocRow.h"
#include <boost/multi_index/random_access_index.hpp>
#include "DocRowsIndex.h"

class HentsDocsListContent : public ListContent,public ForwardHerdObserver
{
	typedef DocRowsIndex<HentsDocRow> HentsDocRowIndex;	
	
public:
	HentsDocsListContent();
	~HentsDocsListContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	HentsDocRow* GetDocAtRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	uint32_t GetHerdId() const;
	void Create(SeasonSession* pSession,uint32_t herdId);
	void Destroy();
protected:
	void AddRow(HentsDocRow* row);
	void RemoveRow(uint32_t docId);
	virtual void CreateContent() = 0;	
protected:
	HentsDocRowIndex m_Rows;
	SeasonSession* m_pSession;
	uint32_t m_herdId;
};


