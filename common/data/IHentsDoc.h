#ifndef __IHENTSDOC_H__
#define __IHENTSDOC_H__

#include "IDoc.h"

class IHentsDoc : public IDoc
{
public:
	virtual uint32_t				GetHerdId() const = 0;
	virtual uint32_t 				GetHentId() const = 0;
	virtual uint32_t				GetInvoiceId() const = 0;
	virtual DocType					GetDocType() const = 0;
};

#endif