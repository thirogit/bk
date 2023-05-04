#ifndef __HENTSDOCSCANDIDATESLISTCONTENT_H__
#define __HENTSDOCSCANDIDATESLISTCONTENT_H__

#include "../../context/ForwardSeasonObserver.h"
#include "SelectableRow.h"
#include "../../content/HentsDocRow.h"
#include "../../ctrls/contentlistctrl/ListContent.h"
#include "../../content/DocRowsIndex.h"
#include "../../context/SeasonSession.h"

typedef SelectableRow<HentsDocRow> SelectableHentsDocRow;

typedef boost::any_range<IHentsDoc*,boost::forward_traversal_tag,IHentsDoc*, std::ptrdiff_t> HentsDocsRange;

class HentsDocsCandidatesListContent : public ListContent,public ForwardSeasonObserver
{
	typedef DocRowsIndex<SelectableHentsDocRow> CandidatesRowIndex;	
	
public:
	HentsDocsCandidatesListContent();
	~HentsDocsCandidatesListContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	
	SelectableHentsDocRow* GetDocAtRow(int row);	
	void Create(SeasonSession* pSession);

	void AddDoc(uint32_t docId);
	void RemoveDoc(uint32_t docId);
	void Destroy();
protected:
	virtual void CreateContent() = 0;
	virtual IHentsDoc* GetHentsDoc(uint32_t docId) = 0;
protected:
	CandidatesRowIndex m_Rows;
	SeasonSession* m_pSession;	
};

#endif