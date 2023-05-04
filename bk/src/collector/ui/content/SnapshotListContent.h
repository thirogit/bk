#ifndef __SNAPSHOTLISTCONTENT_H__
#define __SNAPSHOTLISTCONTENT_H__

#include "../../../ctrls/contentlistctrl/ListContent.h"
#include "../../../ctrls/contentlistctrl/NullListRow.h"
#include "../../../ctrls/contentlistctrl/ContentListCtrl.h"
#include <better-enums\enum.h>
#include "../../data/DownloadSnapshotHeader.h"


BETTER_ENUM(DownloadSnapshotHeaderMember, uint8_t, User,UploadDtTm,DownloadDtTm,HerdIndex)


class SnapshotListRow : public NullListRow
{
public:
	SnapshotListRow(std::unique_ptr<DownloadSnapshotHeader>& snapshot);
	~SnapshotListRow();
	virtual std::wstring CellValue(uint32_t colId);
	virtual Color CellColor(uint32_t colId);
	const DownloadSnapshotHeader* Content() const;
private:
	DownloadSnapshotHeader* m_Content;
};

class SnapshotListContent : public ListContent
{
	struct tag_snapshotId {};

	struct RowIdExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(SnapshotListRow* pRow) const
		{
			return pRow->Content()->GetId();
		}
	};

	typedef boost::multi_index_container
		<
		SnapshotListRow*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<tag_snapshotId>, RowIdExtractor >
		>
		> RowsIndex;

public:
	SnapshotListContent();
	~SnapshotListContent();

	void Add(const DownloadSnapshotHeader* snapshot);
	void Remove(uint32_t id);
	void DestroyContent();
	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	
	const DownloadSnapshotHeader* GetSnapshotRowAt(int iIndex);
protected:
	SnapshotListRow* FindRow(uint32_t id);
private:
	RowsIndex m_Content;
};


#endif