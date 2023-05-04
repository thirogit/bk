#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>

struct herdindex_tag_herdId{};
struct herdindex_tag_herdIndex{};

typedef boost::multi_index_container
<
	Herd*,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<herdindex_tag_herdId>, boost::multi_index::const_mem_fun<Herd, uint32_t, &Herd::GetId> >,
		boost::multi_index::ordered_unique < boost::multi_index::tag<herdindex_tag_herdIndex>, boost::multi_index::const_mem_fun<Herd, int, &Herd::GetHerdIndex> >
	>
> HerdsIndex;;

