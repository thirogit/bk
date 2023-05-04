 #include "stdafx.h"
#include "AliorFormatter.h"
#include "fields/Elixir4x35Field.h"
#include "fields/MoneyCentValueField.h"
#include "fields/QuotedField.h"
#include "fields/InvoiceHentNameAndAddrField.h"
#include "fields/InvoiceHentAccountNoField.h"
#include "fields/CompanySourceAccountNo.h"
#include "fields/MaxSizeField.h"
#include "fields/InvoiceHentZipField.h"
#include "fields/InvoiceHentCityField.h"
#include "fields/MoneyTransferDateField.h"
#include "fields/ConstantField.h"
#include "fields/MoneyTransferTitleField.h"
#include "fields/EmptyField.h"
#include "fields/InvoiceHentBankNoField.h"
#include "fields/CompanySourceBankNo.h"
#include "fields/CompanyNameAndAddrField.h"
#include "fields/ReplaceField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AliorFormatter::AliorFormatter() : DelimetedFormatter(',',L"CSV",L"Alior Bank")
{
	ReplaceRules replaceRules;
	replaceRules.Add(ReplaceRule(L'"',L' ')).Add(ReplaceRule(L',',L' '));
	

	AddField(new ConstantField(L"110"));
	AddField(new MoneyTransferDateField(DateTime::YYYYMMDD));
	AddField(new MoneyCentValueField());
	AddField(new CompanySourceBankNo());
	AddField(new ConstantField(L"0"));
	AddField(new QuotedField(new CompanySourceAccountNo()));
	AddField(new QuotedField(new InvoiceHentAccountNoField()));
	AddField(new QuotedField(new ReplaceField(new Elixir4x35Field(new CompanyNameAndAddrField()),replaceRules)));
	AddField(new QuotedField(new ReplaceField(new Elixir4x35Field(new InvoiceHentNameAndAddrField()),replaceRules)));
	AddField(new ConstantField(L"0"));
	AddField(new InvoiceHentBankNoField());
	AddField(new QuotedField(new ReplaceField(new Elixir4x35Field(new MoneyTransferTitleField()),replaceRules)));
	AddField(new QuotedField(new EmptyField()));
	AddField(new QuotedField(new EmptyField()));
	AddField(new QuotedField(new ConstantField(L"51")));		
}
