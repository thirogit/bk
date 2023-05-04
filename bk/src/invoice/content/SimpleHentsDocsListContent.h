#ifndef __SIMPLEHENTSDOCSLISTCONTENT_H__
#define __SIMPLEHENTSDOCSLISTCONTENT_H__

#include "../../content/DocRowsIndex.h"
#include "../../content/HentsDocRow.h"
#include "../../ctrls/contentlistctrl/ListContent.h"
#include "../../context/SeasonSession.h"

class SimpleHentsDocsListContent : public ListContent
{
	typedef DocRowsIndex<HentsDocRow> HentsDocsRowIndex;	
public:
	SimpleHentsDocsListContent(SeasonSession* pSession);
	~SimpleHentsDocsListContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	HentsDocRow* GetDocAtRow(int row);
	void AddDoc(IHentsDoc* pDoc);
	void RemoveDoc(uint32_t docId);
	void RemoveAllDocs();	
protected:
	HentsDocsRowIndex m_Rows;
	SeasonSession* m_pSession;
};


#endif