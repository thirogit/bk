#ifndef __INVOICEKIND_H__
#define __INVOICEKIND_H__

#include "InvoiceType.h"
#include "InvoiceDirection.h"

class InvoiceKind
{
public:
	InvoiceKind();
	InvoiceKind(const InvoiceKind& src);
	InvoiceKind& operator=(const InvoiceKind& right);
	
	InvoiceKind(InvoiceDirection direction,InvoiceType type);
	InvoiceDirection GetDirection() const;
	InvoiceType GetInvoiceType() const;
	
	void SetDirection(InvoiceDirection direction);
	void SetInvoiceType(InvoiceType type);
private:
	InvoiceDirection m_direction;
	InvoiceType m_type;
};



#endif