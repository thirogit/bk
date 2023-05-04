#ifndef __PAYWAYCOMBO_H__
#define __PAYWAYCOMBO_H__

#include "../BaseCombo.h"
#include <data\IInvoice.h>

class CPaywayCombo : public CComboBox
{
public:
	void ReloadCombo(PayWay payWay = PayWay_None);
	PayWay GetSelectedPayWay();
};


#endif