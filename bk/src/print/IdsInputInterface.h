#ifndef __IDSINPUTINTERFACE_H__
#define __IDSINPUTINTERFACE_H__

//#include "../view/FatOwnDocViewCreator.h"
//#include <boost/range/any_range.hpp>

//typedef boost::any_range<const FatOwnDocView*,boost::forward_traversal_tag, const FatOwnDocView*, std::ptrdiff_t> FatOwnDocViewRange;

#include "../context/SeasonSession.h"
//#include <boost/range/any_range.hpp>
#include "../runtime/UniqueIds.h"

class IdsInputInterface
{
public:
	virtual void Feed(SeasonSession* pSession,const UniqueIds& ids) = 0;
};


#endif
