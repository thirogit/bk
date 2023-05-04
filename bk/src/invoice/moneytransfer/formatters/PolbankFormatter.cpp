 #include "stdafx.h"
#include "PolbankFormatter.h"
#include "fields/InvoiceIdField.h"
#include "fields/MoneyFloatValueField.h"
#include "fields/InvoiceHentNameField.h"
#include "fields/InvoiceHentStreetAddrField.h"
#include "fields/InvoiceHentAccountNoField.h"
#include "fields/CompanySourceAccountNo.h"
#include "fields/MaxSizeField.h"
#include "fields/InvoiceHentZipField.h"
#include "fields/InvoiceHentCityField.h"
#include "fields/MoneyTransferDateField.h"
#include "fields/ConstantField.h"
#include "fields/InvoiceTitleField.h"
#include "fields/InvoiceNumberField.h"
#include "fields/InvoiceDateField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PolbankFormatter::PolbankFormatter() : DelimetedFormatter(';',L"CSV",L"Polbank")
{
	AddField(new CompanySourceAccountNo());
	AddField(new InvoiceIdField());
	AddField(new InvoiceHentAccountNoField());
	AddField(new MaxSizeField(new InvoiceHentNameField(),35));
	AddField(new MaxSizeField(new InvoiceHentZipField(),35));
	AddField(new MaxSizeField(new InvoiceHentCityField(),35));
	AddField(new MaxSizeField(new InvoiceHentStreetAddrField(),35));	
	AddField(new MoneyFloatValueField(DecimalFormat(2,L',')));
	AddField(new MoneyTransferDateField(DateTime::DDSMMSYYYY));
	
	AddField(new MaxSizeField(new InvoiceTitleField(),35));
	AddField(new MaxSizeField(new InvoiceNumberField(),35));
	AddField(new MaxSizeField(new InvoiceDateField(DateTime::DDdashMMdashYYYY),35));
	AddField(new ConstantField(L"."));
		
}
