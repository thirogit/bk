#ifndef __LISTHEADERLAYOUT_H__
#define __LISTHEADERLAYOUT_H__

#include <cstdint>
#include <arrays\NewPtrFlushArray.h>
#include "ColumnLayout.h"
#include <arrays\Enumerator.h>


class ListHeaderLayout
{
public:
	ListHeaderLayout();
	~ListHeaderLayout();
	ListHeaderLayout(const ListHeaderLayout& layout);
	ListHeaderLayout& operator=(const ListHeaderLayout& right);
	void AddColumnLayout(uint32_t colId, int colWidth);
	void RemoveColumnLayout(uint32_t colId);
	PtrEnumerator<ColumnLayout> EnumColumns();
	bool IsEmpty() const;
	ColumnLayout* GetColumnAt(int nCol);
	void MoveColumn(int srcIndex, int dstIndex);
	int GetColumnCount() const;
	bool ContainsColumnWithId(uint32_t colId);
	void Clear();
private:
	NewPtrFlushArray<ColumnLayout> m_ColumnLayouts;
};

#endif