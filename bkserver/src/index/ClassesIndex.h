#pragma once

#include <data/impl/Class.h>

struct classesindex_tag_id {};
struct classesindex_tag_code {};

typedef boost::multi_index_container
<
Class*,
boost::multi_index::indexed_by<
boost::multi_index::ordered_unique < boost::multi_index::tag<classesindex_tag_id>, boost::multi_index::const_mem_fun<Class, uint32_t, &Class::GetId> >,
boost::multi_index::ordered_unique < boost::multi_index::tag<classesindex_tag_code>, boost::multi_index::const_mem_fun<Class, const std::wstring&, &Class::GetClassCode> >
>
> ClassesIndex;

