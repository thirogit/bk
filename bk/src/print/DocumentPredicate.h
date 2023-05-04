#ifndef __DOCUMENTPREDICATE_H__
#define __DOCUMENTPREDICATE_H__

#include "../view/InvoiceView.h"

class DocumentPredicate
{	
public:	
	virtual bool applies(const InvoiceView* pView) = 0;
};

#endif
