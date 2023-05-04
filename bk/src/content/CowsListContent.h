#pragma once
#include "..\ctrls\contentlistctrl\ListContent.h"
#include "CowPresenceRow.h"
#include "../context/ForwardHerdObserver.h"
#include "../context/SeasonSession.h"
#include <boost/multi_index/random_access_index.hpp>
#include "FindResult.h"
#include "predicate\Predicate.h"

class CowsListContent : public ListContent,public ForwardHerdObserver
{

	struct tag_Key{};

	struct RowKeyExtractor
	{
		typedef const CowPresenceKey& result_type;
		result_type operator()(CowPresenceRow* pRow) const 
		{
			return pRow->Content()->GetKey();
		}
	};

	typedef boost::multi_index_container
	<
		CowPresenceRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_Key>, RowKeyExtractor >
										>
	> IndexType;


public:
	CowsListContent();
	~CowsListContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);

	CowPresenceRow* GetCowAtRow(int row);
	uint32_t GetHerdId() const;	
	void Create(SeasonSession* pSession,uint32_t herdId);
	FindResult Find(Predicate<CowPresenceView>* predicate);
	void Destroy();
	void ClearFound();
	void SortFound();
protected:
	virtual void CreateContent() = 0;
	void AddRow(CowPresenceRow* row);
	void RefreshCowRow(const CowPresenceKey& rowKey);	
	void RemoveRow(const CowPresenceKey& rowKey);

	virtual void  OnHerdCowLocked(const CowPresenceKey& key);
	virtual void  OnHerdCowUnlocked(const CowPresenceKey& key);				
	virtual void  OnHerdCowUpdated(const CowPresenceKey& key);
	virtual void  OnHerdCowAdded(const CowPresenceKey& key);
	virtual void  OnHerdCowDeleted(const CowPresenceKey& key);

protected:	
	IndexType m_Rows;
	uint32_t m_herdId;
	SeasonSession* m_pSession;	
};


