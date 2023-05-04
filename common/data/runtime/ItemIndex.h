#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include "IItem.h"

template<class ITEM>
struct ItemIndex
{
	struct tag_ItemId{};

	typedef boost::multi_index_container
	<
		ITEM*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_ItemId>, boost::multi_index::const_mem_fun<IItem,ItemId, &IItem::GetId> >
										>
	> IndexType;


	typedef ITEM* value_type;

	typedef typename IndexType::const_iterator const_iterator;
	typedef typename IndexType::iterator iterator;
	typedef typename boost::multi_index::index<IndexType, tag_ItemId>::type IndexByItemId;

	ITEM* remove_item(ItemId id)
	{
		IndexByItemId&  index = boost::multi_index::get<tag_ItemId>(m_index);
		auto it = index.find(id);	
		if(it != index.end())
		{
			ITEM* item = *it;
			index.erase(it);	
			return item;		
		}

		return NULL;
	}

	bool contains_item(ItemId id)
	{
		return find_item(id) != NULL;
	}

	ITEM* find_item(ItemId id)
	{
		IndexByItemId&  index = boost::multi_index::get<tag_ItemId>(m_index);
		auto it = index.find(id);	
		if(it != index.end())
		{
			return *it;
		}
		return NULL;
	}

	void add_item(ITEM* item)
	{
		m_index.push_back(item);
	}

	void clear()
	{
		m_index.clear();
	}

	size_t size() const
	{
		return m_index.size();
	}


	const_iterator begin() const
	{
		return m_index.begin();
	}

	const_iterator end() const
	{
		return m_index.end();
	}

	iterator begin()
	{
		return m_index.begin();
	}

	iterator end()
	{
		return m_index.end();
	}


	ITEM* item_at(int index)
	{			
		return m_index[index];		
	}

	template<typename CMPTR>
	void sort(CMPTR& comparator)
	{
		m_index.sort(comparator);
	}
private:
	IndexType m_index;
};

	
