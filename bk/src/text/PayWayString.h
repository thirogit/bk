#ifndef __PAYWAYSTRING_H__
#define __PAYWAYSTRING_H__

#include <data/IInvoice.h>
#include <string>

class PayWayString : private std::wstring
{
public:	
	PayWayString(PayWay payWay);
	const std::wstring& ToString();
};

#endif