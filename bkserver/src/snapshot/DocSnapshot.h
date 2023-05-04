#pragma once

#include <arrays/NewPtrFlushArray.h>
#include <data/impl/DocDetails.h>
#include "DocCowItem.h"
#include <boost/range/any_range.hpp>

typedef boost::any_range<const DocCowItem*,boost::forward_traversal_tag,DocCowItem* const, std::ptrdiff_t> DocSnapshotItemsRange;

class DocSnapshot
{
public:
	DocSnapshot(uint32_t docId);
	~DocSnapshot();
	uint32_t GetDocId() const;
	
	const DocDetails& GetDetails() const;
	void SetDetails(const DocDetails& details);
	
	size_t GetItemsCount() const;
	const DocCowItem& GetItem(size_t index) const;
	void AddItem(const DocCowItem& item);	

	DocSnapshotItemsRange Items() const;
	
	virtual std::wstring ToString() const;

private:
	uint32_t m_docId;
	DocDetails m_detais;
	NewPtrFlushArray<DocCowItem> m_Items;
};
