#ifndef __COWCANDIDATESCONTENT_H__
#define __COWCANDIDATESCONTENT_H__

#include "SelectableRow.h"
#include "../../content/Row.h"
#include "../../view/CowView.h"
#include "../../text/CowViewText.h"

#include <boost/multi_index/random_access_index.hpp>
#include "../../ctrls/contentlistctrl/ListContent.h"
#include "../../context/SeasonSession.h"
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>


typedef Row<CowView,CowViewText> CowRow;
typedef SelectableRow<CowRow> CowSelectableRow;

class CowCandidatesContent : public ListContent
{	
	struct tag_cowId {};

	struct RowCowIdExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(CowRow* pRow) const 
		{
			return pRow->Content()->GetId();
		}
	};

	typedef boost::multi_index_container
	<
		CowSelectableRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,										
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_cowId>, RowCowIdExtractor >
										>
	> RowsIndex;


public:
	void CreateContent(SeasonSession* session); 
	void DestroyContent();	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	
	void RemoveRow(int row);

	void RemoveCow(uint32_t cowId);
	void AddCow(uint32_t cowId);
	
	CowSelectableRow* GetRowAt(int row);	
		
protected:
	CowSelectableRow* FindRow(uint32_t cowId);
	
	void ClearContent();	
	void AddRow(CowSelectableRow* pRow);	
private:
	SeasonSession* m_pSession;	
	RowsIndex m_Rows;
};



#endif
