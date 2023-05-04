#include "stdafx.h"
#include "ElixirFormatter.h"
#include "fields/MoneyTransferTitleField.h"
#include "fields/MoneyCentValueField.h"
#include "fields/MoneyTransferDateField.h"
#include "fields/InvoiceHentAddrField.h"
#include "fields/CurrencyCodeField.h"
#include "fields/InvoiceHentAccountNoField.h"
#include "fields/ConstantField.h"
#include <types/DateTime.h>
#include "fields/Elixir4x35Field.h"
#include "fields/EmptyField.h"
#include "fields/QuotedField.h"
#include "fields/InvoiceHentNameAndAddrField.h"
#include "fields/CompanySourceBankNo.h"
#include "fields/CompanySourceAccountNo.h"
#include "fields/InvoiceHentBankNoField.h"

ElixirFormatter::ElixirFormatter() : DelimetedFormatter(',',L"CSV",L"Elixir-0")
{
	AddField(new ConstantField(L"110"));
	AddField(new MoneyTransferDateField(DateTime::YYYYMMDD));
	AddField(new MoneyCentValueField());
	AddField(new CompanySourceBankNo());
	AddField(new ConstantField(L"0"));
	AddField(new QuotedField(new CompanySourceAccountNo()));
	AddField(new QuotedField(new InvoiceHentAccountNoField()));
	AddField(new QuotedField(new EmptyField()));
	AddField(new QuotedField(new Elixir4x35Field(new InvoiceHentNameAndAddrField())));
	AddField(new ConstantField(L"0"));
	AddField(new InvoiceHentBankNoField());
	AddField(new QuotedField(new Elixir4x35Field(new MoneyTransferTitleField())));
	AddField(new QuotedField(new EmptyField()));
	AddField(new QuotedField(new EmptyField()));
	AddField(new QuotedField(new ConstantField(L"51")));
	AddField(new EmptyField());
}
