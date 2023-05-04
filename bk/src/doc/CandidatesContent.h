#ifndef __CANDIDATESCONTENT_H__
#define __CANDIDATESCONTENT_H__

#include "../context/ForwardHerdObserver.h"
#include "CowPresenceSelectRow.h"
#include <boost/multi_index/random_access_index.hpp>
#include "../ctrls/contentlistctrl/ListContent.h"
#include "../context/SeasonSession.h"
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>

class CandidatesContent : public ListContent, public ForwardHerdObserver
{
	struct tag_presenceKey {};
	struct tag_cowId {};

	
	struct RowPresenceKeyExtractor
	{
		typedef CowPresenceKey result_type;
		result_type operator()(CowPresenceSelectRow* pRow) const 
		{
			return pRow->Content()->GetKey();
		}
	};

	struct RowCowIdExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(CowPresenceSelectRow* pRow) const 
		{
			return pRow->Content()->GetKey().GetCowId();
		}
	};

	typedef boost::multi_index_container
	<
		CowPresenceSelectRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_presenceKey>, RowPresenceKeyExtractor >,									
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_cowId>, RowCowIdExtractor >
										>
	> RowsIndex;

public:
	void CreateContent(SeasonSession* session,uint32_t herdId); 
	void DestroyContent();	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);

	void RemoveRow(int row);
	void RemoveCow(uint32_t cowId);
	void AddRow(CowPresenceKey& key);
	CowPresenceSelectRow* FindRow(int row);	
			
	virtual void  OnHerdCowLocked(const CowPresenceKey& key);
	virtual void  OnHerdCowUnlocked(const CowPresenceKey& key);			
	virtual void  OnHerdCowAdded(const CowPresenceKey& key);
	virtual void  OnHerdCowRemoved(const CowPresenceKey& key);
	virtual void  OnHerdCowMoved(const CowPresenceKey& fromKey,const CowPresenceKey& toKey);
	virtual void  OnHerdCowUpdated(const CowPresenceKey& key);
protected:
	CowPresenceSelectRow* FindRow(CowPresenceKey& key);
	void ClearContent();
	
	void RemoveRow(CowPresenceKey& key);
	void AddRow(CowPresenceSelectRow* pRow);	
private:
	uint32_t m_herdId;
	SeasonSession* m_pSession;	
	RowsIndex m_Rows;
};



#endif
