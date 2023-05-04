#ifndef __COWCOMPARATOR_H__
#define __COWCOMPARATOR_H__

#include "../view/CowPresenceView.h"
#include "MemberComparator.h"
#include "HentComparator.h"

class CowPresenceViewComparator : public MemberComparator<const CowPresenceView>
{
public:
	virtual COMPARE_RESULT CompareBy(uint32_t dataMember,const CowPresenceView* DataLeft,const CowPresenceView* DataRight) const;
private:
	SafeComparatorWrapper<HentComparator> m_HentCmp;
};


#endif