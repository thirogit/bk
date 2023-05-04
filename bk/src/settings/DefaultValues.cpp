#include "stdafx.h"
#include "DefaultValues.h"
#include <data\IIdable.h>

DefaultValues::DefaultValues() : 
		stockId(NULL_ID), 
		classId(NULL_ID), 
		paywayRegInvoice(PayWay_None), 
		paywayLumpInvoice(PayWay_None),
		inmotive(Motive_None),
		outmotive(Motive_None),
		henttype(HentType_None),
		bSetInMotive(FALSE),
		bSetOutMotive(FALSE),
		bSetHentType(FALSE),
		bSetStock(FALSE),
		bSetClass(FALSE),
		bSetPayWayRegInvoice(FALSE),
		bSetPayWayLumpInvoice(FALSE)
{

}

DefaultValues::DefaultValues(const DefaultValues& src)
{
	operator=(src);
}

DefaultValues& DefaultValues::operator=(const DefaultValues& right)
{
	this->stockId = right.stockId;
	this->classId = right.classId;
	this->vatRateRegInvoice = right.vatRateRegInvoice;
	this->paywayRegInvoice = right.paywayRegInvoice;
	this->vatRateLumpInvoice = right.vatRateLumpInvoice;
	this->paywayLumpInvoice = right.paywayLumpInvoice;

	this->bSetInMotive = right.bSetInMotive;
	this->bSetOutMotive = right.bSetOutMotive;
	this->bSetHentType = right.bSetHentType;
	this->bSetStock = right.bSetStock;
	this->bSetClass = right.bSetClass;
	this->bSetPayWayRegInvoice = right.bSetPayWayRegInvoice;
	this->bSetPayWayLumpInvoice = right.bSetPayWayLumpInvoice;

	this->inmotive = right.inmotive;
	this->outmotive = right.outmotive;
	this->cowsex = right.cowsex;
	this->henttype = right.henttype;

	this->payDueDaysRegular = right.payDueDaysRegular;
	this->payDueDaysLump = right.payDueDaysLump;

	return *this;
}