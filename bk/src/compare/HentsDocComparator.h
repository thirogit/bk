#ifndef __HENTSDOCCOMPARATOR_H__
#define __HENTSDOCCOMPARATOR_H__

#include "DocComparator.h"
#include "../view/HentsDocView.h"

class HentsDocComparator : public MemberComparator<HentsDocView>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember, HentsDocView* DataLeft, HentsDocView* DataRight) const;
private:
	DocComparator m_comparator;	
	

};


#endif