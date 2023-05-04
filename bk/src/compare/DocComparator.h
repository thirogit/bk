#ifndef __DOCCOMPARATOR_H__
#define __DOCCOMPARATOR_H__

#include "../view/DocView.h"
#include "MemberComparator.h"

class DocComparator : public MemberComparator<DocView>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember, DocView* DataLeft, DocView* DataRight) const;
};


#endif