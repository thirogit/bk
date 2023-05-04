#ifndef __OWNDOCCOMPARATOR_H__
#define __OWNDOCCOMPARATOR_H__

#include "DocComparator.h"
#include "../view/OwnDocView.h"

class OwnDocComparator : public MemberComparator<OwnDocView>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember, OwnDocView* DataLeft, OwnDocView* DataRight) const;
private:
	DocComparator m_comparator;	
};


#endif