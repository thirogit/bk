#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>

struct userindex_tag_userid{};
struct userindex_tag_username{};

typedef boost::multi_index_container
<
	User*,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique < boost::multi_index::tag<userindex_tag_userid>, boost::multi_index::const_mem_fun<User, uint32_t, &User::GetId> >,
		boost::multi_index::ordered_unique < boost::multi_index::tag<userindex_tag_username>, boost::multi_index::const_mem_fun<User, const std::wstring&, &User::GetLogin> >
	>
> UsersIndex;

