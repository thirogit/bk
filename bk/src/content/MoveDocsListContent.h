#pragma once
#include "..\ctrls\contentlistctrl\ListContent.h"
#include "../context/SeasonSession.h"
#include "../context/ForwardHerdObserver.h"
#include "MoveDocRow.h"
#include <boost/multi_index/random_access_index.hpp>


class MoveDocsListContent : public ListContent,public ForwardHerdObserver
{
	struct tag_docId{};

	struct RowKeyExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(MoveDocRow* pRow) const 
		{
			return pRow->Content()->GetDocId();
		}
	};


	typedef boost::multi_index_container
	<
		MoveDocRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_docId>, RowKeyExtractor >
										>
	> RowsIndex;
public:
	MoveDocsListContent();
	~MoveDocsListContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	uint32_t GetHerdId() const;
	MoveDocRow* GetMoveDocAtRow(int row);
	void Create(SeasonSession* pSession,uint32_t herdId);
	void Destroy();
protected:
	virtual void OnHerdMoveDocUpdated(uint32_t docId);
	virtual void OnHerdMoveDocInserted(uint32_t docId);	
	virtual void OnHerdMoveDocDeleted(uint32_t docId);
	virtual void OnHerdMoveDocLocked(uint32_t docId);
	virtual void OnHerdMoveDocUnlocked(uint32_t docId);

	void AddRow(MoveDocRow* row);
	void RemoveRow(uint32_t docId);
	void RefreshRowForDoc(uint32_t docId);
private:
	RowsIndex m_Rows;
	SeasonSession* m_pSession;
	uint32_t m_herdId;
};


