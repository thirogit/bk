#ifndef __ITEMINDEX_H__
#define __ITEMINDEX_H__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include "DocItem.h"

struct index_tag_itemId{};
		
typedef boost::multi_index_container
<
  DocItem*,
  boost::multi_index::indexed_by< 
									boost::multi_index::random_access<>,
									boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_itemId>, boost::multi_index::const_mem_fun<DocItem,ItemId, &DocItem::GetItemId> >										
								>
> DocItemIndex;

#endif