#ifndef __RECOGNITIONSESSIONCONTENT_H__
#define __RECOGNITIONSESSIONCONTENT_H__

#include "..\../../ctrls\contentlistctrl\ListContent.h"
#include "../../session/PassportRecognitionSession.h"
#include "RecognitionItemRow.h"
#include <boost/multi_index/random_access_index.hpp>
#include "../../../context/SeasonSession.h"

class RecognitionSessionContent : public ListContent, public PassportRecognitionObserver
{
	struct tag_Key{};

	struct RowKeyExtractor
	{
		typedef ItemId result_type;
		result_type operator()(RecognitionItemRow* pRow) const
		{
			return pRow->Content()->GetId();
		}
	};

	typedef boost::multi_index_container
		<
		RecognitionItemRow*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<tag_Key>, RowKeyExtractor >
		>
		> IndexType;

public:
	RecognitionSessionContent(SeasonSession* pSeasonSession);
	~RecognitionSessionContent();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);

	RecognitionItemRow* GetItemAtRow(int row);
	void Create(PassportRecognitionSession* pSession);
	void Destroy();
protected:
	void AddRow(RecognitionItemRow* row);
	void RefreshRow(ItemId id);	
	void RemoveRow(ItemId id);

	virtual void OnItemUpdated(ItemId id);
	virtual void OnItemAdded(ItemId id);

protected:	
	IndexType m_Rows;	
	PassportRecognitionSession* m_pRecognitionSession;
	SeasonSession* m_pSeasonSession;
};


#endif