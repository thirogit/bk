#pragma once

#include "..\ctrls\contentlistctrl\ListContent.h"
#include "DocItemRow.h"
#include "../context/SeasonSession.h"
#include "../context/transaction/DocTransactionFacade.h"
#include <map>
#include <boost/multi_index/random_access_index.hpp>

class DocTransactionContent : public ListContent,public TransactionObserver
{
	struct index_tag_itemId{};

	struct RowKeyExtractor
	{
		typedef ItemId result_type;
		result_type operator()(DocItemRow* pRow) const 
		{
			return pRow->Content()->GetItemId();
		}
	};

	typedef boost::multi_index_container
			<
				DocItemRow*,
				boost::multi_index::indexed_by< 
												boost::multi_index::random_access<>,
												boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_itemId>, RowKeyExtractor >
											  >
			> TransactionContentIndex;

public:
	DocTransactionContent();
	~DocTransactionContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	DocItemRow* GetItemRowAt(int row);
	void Create(SeasonSession* pSession,DocTransactionFacade* transaction);
	virtual void SortContent(const SortAttributes& attributes);
	void Destroy();
protected:
	void AddRow(DocItemRow* row);
	void RefreshItemRow(ItemId id);
	DocItemRow* FindRow(ItemId id);


	virtual void OnItemUpdated(ItemId id);
	virtual void OnItemAdded(ItemId id);
	virtual void OnItemDeleted(ItemId id);
private:	
	TransactionContentIndex m_Rows;
	DocTransactionFacade* m_transaction;
	SeasonSession* m_pSession;	
};


