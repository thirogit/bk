#include "stdafx.h"
#include "BZWBKFormatter.h"
#include "fields/MoneyTransferTitleField.h"
#include "fields/MoneyFloatValueField.h"
#include "fields/InvoiceHentNameAndNoField.h"
#include "fields/InvoiceHentAddrField.h"
#include "fields/CurrencyCodeField.h"
#include "fields/InvoiceHentAccountNoField.h"
#include "fields/ConstantField.h"
#include "fields/MoneyTransferDateField.h"
#include "fields/MaxSizeField.h"
#include "fields/CompanySourceAccountNo.h"
#include "fields/InvoiceHentCityField.h"
#include "fields/InvoiceHentStreetAddrField.h"
#include "fields/InvoiceHentZipField.h"
#include <format/StringFormat.h>
#include "fields/EmptyField.h"

BZWBKFormatter::BZWBKFormatter() : DelimetedFormatter(L'|',L"CSV",L"BZ WBK")
{
	AddField(new ConstantField(L"1"));
	AddField(new CompanySourceAccountNo());
	AddField(new InvoiceHentAccountNoField());
	AddField(new MaxSizeField(new InvoiceHentNameAndNoField(),80));
	AddField(new MaxSizeField(new InvoiceHentAddrField(),60));
	AddField(new MoneyFloatValueField(DecimalFormat(2,L',')));
	AddField(new ConstantField(L"1"));
	AddField(new MaxSizeField(new MoneyTransferTitleField(),140));
	AddField(new EmptyField());
}

std::string BZWBKFormatter::GetDefaultEncoding()
{
	return "windows-1250";
}

void BZWBKFormatter::FormatTransfers(MoneyTransferEntryRange moneyTransfers,
										MoneyTransferContext* pContext,
										std::wofstream& output,
										SimpleProgress *progress)
{
	output << L"4120414";
	WriteDelimiter(output);
	output << "1";
	WriteLineEnd(output);
	DelimetedFormatter::FormatTransfers(moneyTransfers, pContext, output, progress);
}