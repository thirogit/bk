#pragma once

template<typename SO>
struct ServerObjectPeeler
{
    typedef typename SO::object_type* result_type;
    result_type operator()(SO* so) const { return so->Object(); }
};
