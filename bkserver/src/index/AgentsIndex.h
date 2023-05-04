#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>

struct agentsindex_tag_id {};
struct agentsindex_tag_code {};

typedef boost::multi_index_container
<
Agent*,
boost::multi_index::indexed_by<
boost::multi_index::ordered_unique < boost::multi_index::tag<agentsindex_tag_id>, boost::multi_index::const_mem_fun<Agent, uint32_t, &Agent::GetId> >,
boost::multi_index::ordered_unique < boost::multi_index::tag<agentsindex_tag_code>, boost::multi_index::const_mem_fun<Agent, const std::wstring&, &Agent::GetAgentCode> >
>
> AgentsIndex;

