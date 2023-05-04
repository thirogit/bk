#ifndef __INVOICEITEMINDEX_H__
#define __INVOICEITEMINDEX_H__

#include "InvoiceTransactionItem.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <vector>

class InvoiceItemIndex 
{
	struct tag_itemId{};
	//struct tag_itemKey{};
	struct tag_cowId{};
	struct tag_docId{};

	struct ItemIdKeyExtractor
	{
		typedef ItemId result_type;
		result_type operator()(InvoiceTransactionItem* item) const
		{
			return item->GetItemKey().GetItemId();
		}
	};

	struct CowIdKeyExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(InvoiceTransactionItem* item) const
		{
			return item->GetCowId();
		}
	};

	struct DocIdKeyExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(InvoiceTransactionItem* item) const
		{
			return item->GetItemKey().GetDocKey().GetDocNo();
		}
	};

	typedef boost::multi_index_container
	<
		InvoiceTransactionItem*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_itemId>, ItemIdKeyExtractor >,
										//boost::multi_index::ordered_unique < boost::multi_index::tag<tag_itemKey>, boost::multi_index::const_mem_fun<InvoiceItem,const InvoiceItemKey&, &InvoiceItem::GetItemKey> >,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_cowId>, CowIdKeyExtractor >,
										boost::multi_index::ordered_non_unique < boost::multi_index::tag<tag_docId>, DocIdKeyExtractor >
										>
	> Index;
	
public:
	typedef InvoiceTransactionItem* value_type;

	typedef Index::const_iterator const_iterator;
	typedef Index::iterator iterator;
	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();

	InvoiceTransactionItem* find_item(uint32_t cowId);
	InvoiceTransactionItem* find_item(ItemId id);
	void push_back(InvoiceTransactionItem* item);
	size_t size() const;
	InvoiceTransactionItem* get_at(int index);
	std::vector<InvoiceTransactionItem*> delete_items(uint32_t docId);
	InvoiceTransactionItem* delete_item(ItemId id);
private:
	Index m_items;

};



#endif