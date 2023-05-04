#ifndef __MOVEDOCCOMPARATOR_H__
#define __MOVEDOCCOMPARATOR_H__

#include "DocComparator.h"
#include "../view/MoveDocView.h"

class MoveDocComparator : public MemberComparator<MoveDocView>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember, MoveDocView* DataLeft, MoveDocView* DataRight) const;
private:
	DocComparator m_comparator;	
};


#endif