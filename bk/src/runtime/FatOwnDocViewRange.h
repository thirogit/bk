#ifndef __FATOWNDOCVEWRANGE_H__
#define __FATOWNDOCVEWRANGE_H__

#include "../view/FatOwnDocView.h"
#include <boost/range/any_range.hpp>

typedef boost::any_range<const FatOwnDocView*,boost::forward_traversal_tag, const FatOwnDocView*, std::ptrdiff_t> FatOwnDocViewRange;

#endif
