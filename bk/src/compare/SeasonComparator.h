#ifndef __SEASONCOMPARATOR_H__
#define __SEASONCOMPARATOR_H__

#include <data\ISeason.h>
#include "MemberComparator.h"

enum SeasonMemeber
{
	SeasonMember_Name,
	SeasonMember_StartDt,
	SeasonMember_EndDt
};

class SeasonComparator : public MemberComparator<ISeason>
{
public:
	virtual COMPARE_RESULT CompareBy(uint32_t memberId,ISeason* DataLeft,ISeason* DataRight) const;
};


#endif