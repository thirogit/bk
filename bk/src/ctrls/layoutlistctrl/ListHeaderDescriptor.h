#ifndef __LISTHEADERDESCRIPTOR_H__
#define __LISTHEADERDESCRIPTOR_H__

#include <arrays\Enumerator.h>
#include <stdint.h>
#include "HeaderColumn.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

class ListHeaderDescriptor
{
	struct index_tag_id {};

	typedef boost::multi_index_container
		<
		HeaderColumn*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<HeaderColumn, uint32_t, &HeaderColumn::GetColId> >
		>
		> ColumnIndex;

public:
	ListHeaderDescriptor();
	~ListHeaderDescriptor();
	ListHeaderDescriptor(const ListHeaderDescriptor& src);

	void AddColumn(const HeaderColumn& column);
	ListHeaderDescriptor& operator()(uint32_t colId,const std::wstring& colText);
	PtrEnumerator<HeaderColumn> EnumColumns() const;
	const HeaderColumn* GetColumnWithId(uint32_t) const;
	
	ListHeaderDescriptor& operator=(const ListHeaderDescriptor& right);
private:
	ColumnIndex m_Columns;
};

#endif