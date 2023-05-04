#pragma once

#include <data/impl/Stock.h>


struct stocksindex_tag_id {};
struct stocksindex_tag_code {};

typedef boost::multi_index_container
<
Stock*,
boost::multi_index::indexed_by<
boost::multi_index::ordered_unique < boost::multi_index::tag<stocksindex_tag_id>, boost::multi_index::const_mem_fun<Stock, uint32_t, &Stock::GetId> >,
boost::multi_index::ordered_unique < boost::multi_index::tag<stocksindex_tag_code>, boost::multi_index::const_mem_fun<Stock, const std::wstring&, &Stock::GetStockCode> >
>
> StocksIndex;

