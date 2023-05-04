#ifndef __PASSPORTCOLLECTION_H__
#define __PASSPORTCOLLECTION_H__

#include <boost/range/any_range.hpp>
#include "CowPassport.h"

typedef boost::any_range<CowPassport,boost::forward_traversal_tag,const CowPassport> CowPassportRange;

class PassportCollection
{
public:
	virtual CowPassportRange Enum() const = 0;
};

#endif