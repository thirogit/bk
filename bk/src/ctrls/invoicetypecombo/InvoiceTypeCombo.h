#ifndef __INVOICETYPECOMBO_H__
#define __INVOICETYPECOMBO_H__

#include "../BaseCombo.h"
#include <data\IInvoice.h>

class CInvoiceTypeCombo : public CComboBox
{
public:
	void ReloadCombo(InvoiceType type = InvoiceType_None);
	InvoiceType GetSelectedType();
};


#endif