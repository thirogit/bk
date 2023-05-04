#pragma once

#include <stdint.h>
#include <types/NullDecimal.h>
#include <data\PayWay.h>
#include <data\Motive.h>
#include <types\NullInt.h>
#include <data\HentType.h>
#include <data\types\CowSex.h>
#include <data\InvoiceType.h>
#include <wtypes.h>

class DefaultValues
{
public:
	DefaultValues();
	DefaultValues(const DefaultValues& src);
	DefaultValues& operator=(const DefaultValues& right);

	CowSex       cowsex;
	
	uint32_t	 stockId;
	bool         bSetStock;

	uint32_t	 classId;
	bool         bSetClass;

	HentType     henttype;
	bool         bSetHentType;

	Motive       inmotive;
	bool         bSetInMotive;

	Motive       outmotive;
	bool         bSetOutMotive;

	NullDecimal   vatRateRegInvoice;

	PayWay       paywayRegInvoice;
	bool		 bSetPayWayRegInvoice;

	NullDecimal   vatRateLumpInvoice;

	PayWay 		 paywayLumpInvoice;
	bool		 bSetPayWayLumpInvoice;

	NullInt      payDueDaysRegular;	
	bool		 bSetPayDueDaysRegInvoice;

	NullInt      payDueDaysLump;
	bool		 bSetPayDueDaysLumpInvoice;

};


	

