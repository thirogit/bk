#ifndef __FORWARDDOCUMENTPREDICATE_H__
#define __FORWARDDOCUMENTPREDICATE_H__

#include "DocumentPredicate.h"

class ForwardDocumentPredicate : public DocumentPredicate
{		
public:
	virtual bool applies(const InvoiceView* pView);
};


#endif
