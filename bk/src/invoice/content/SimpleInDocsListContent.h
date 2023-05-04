#ifndef __SIMPLEINDOCSLISTCONTENT_H__
#define __SIMPLEINDOCSLISTCONTENT_H__

#include "../../content/DocRowsIndex.h"
#include "../../content/OwnDocRow.h"
#include "../../ctrls/contentlistctrl/ListContent.h"
#include "../../context/SeasonSession.h"

class SimpleInDocsListContent : public ListContent
{
	typedef DocRowsIndex<OwnDocRow> OwnDocsRowIndex;	
public:
	SimpleInDocsListContent(SeasonSession* pSession);
	~SimpleInDocsListContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	OwnDocRow* GetDocAtRow(int row);
	void AddDoc(IOwnDoc* pDoc);
	void RemoveDoc(uint32_t docId);
	void RemoveAllDocs();	
protected:
	OwnDocsRowIndex m_Rows;
	SeasonSession* m_pSession;
};


#endif