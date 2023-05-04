#ifndef __INVOICEVIEWCOMPARATOR_H__
#define __INVOICEVIEWCOMPARATOR_H__

#include "../view/InvoiceView.h"
#include "MemberComparator.h"
#include "HentComparator.h"

class InvoiceViewComparator : public MemberComparator<const InvoiceView>
{
public:
	virtual COMPARE_RESULT CompareBy(uint32_t dataMember,const InvoiceView* DataLeft,const InvoiceView* DataRight) const;
private:
	SafeComparatorWrapper<HentComparator> m_HentCmp;
};


#endif