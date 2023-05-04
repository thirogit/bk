#ifndef __HENTCOMPARATOR_H__
#define __HENTCOMPARATOR_H__

#include "..\view\HentView.h"
#include "MemberComparator.h"

class HentComparator : public MemberComparator<const HentView>
{
public:
	virtual COMPARE_RESULT CompareBy(uint32_t memberId,const HentView* DataLeft,const HentView* DataRight) const;
};


#endif