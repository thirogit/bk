#ifndef __COLLECTORLISTCONTENT_H__
#define __COLLECTORLISTCONTENT_H__

#include "../../../ctrls/contentlistctrl/ListContent.h"
#include "../../../ctrls/contentlistctrl/NullListRow.h"
#include "../text/CollectorText.h"
#include "../../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "../../Collector.h"


class CollectorListRow : public NullListRow
{
public:
	CollectorListRow(const ICollector* collector);
	~CollectorListRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	Collector* Content() const;
	
private:
	CollectorText m_CollectorText;
	Collector* m_Content;
};

class CollectorListContent : public ListContent
{
	struct tag_collectorId {};

	struct RowIdExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(CollectorListRow* pRow) const
		{
			return pRow->Content()->GetId();
		}
	};

	typedef boost::multi_index_container
		<
		CollectorListRow*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<tag_collectorId>, RowIdExtractor >
		>
		> RowsIndex;

public:
	CollectorListContent();
	~CollectorListContent();

	void AddCollector(const ICollector* collector);
	void RemoveCollector(uint32_t id);
	void DestroyContent();
	void UpdateCollector(const ICollector* collector);
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	ICollector* GetCollectorAt(int iIndex);
protected:
	CollectorListRow* FindRow(uint32_t id);
private:
	RowsIndex m_Content;
};


#endif