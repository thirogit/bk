#include "stdafx.h"
#include "JpkExporter.h"

#include <boost/foreach.hpp>
#include "../../../invoice/calculator/InvoiceCalculator.h"
#include <string/TextUtils.h>

static const wchar_t* _tns = L"http://jpk.mf.gov.pl/wzor/2017/11/13/1113/";
static const DecimalFormat _decimalFormat(2, L'.');
static const wchar_t* _none = L"BRAK";

std::wstring JpkExporter::GetExporterName()
{
	return L"JPK";
}

std::wstring JpkExporter::GetFileExtension()
{
	return L"xml";
}

std::string JpkExporter::GetDefaultEncoding()
{
	return "utf-8";
}



void JpkExporter::Export(IInvoiceRange invoices,
									 InvoiceExporterOptions& options,
									 std::wofstream& output,
									 SeasonSession* session,
									 SimpleProgress *progress)
{	
	
	using namespace quickxml;
		

	int buyIndex = 0;
	int sellIndex = 0;


	std::list<element> sells;
	Decimal buyTax, sellTax;
	std::list<element> buys;

	DateTime minDate, maxDate;

	BOOST_FOREACH(IInvoice* invoice, invoices)
	{	
		InvoiceCalculator::CalculationResult calculation = InvoiceCalculator::Calculate(invoice);
		if (invoice->GetInvoiceDirection() == InvoiceDirection_Buy)
		{
			buys.push_back(SerializeBuy(buyIndex+1, (IBuyInvoice*)invoice));
			buyIndex++;
			buyTax += calculation.GetTaxValue().Round(2);
		}
		else
		{
			sells.push_back(SerializeSell(sellIndex+1, (ISellInvoice*)invoice));
			sellIndex++;
			sellTax += calculation.GetTaxValue().Round(2);
		}


		if (minDate.IsNull() || minDate > invoice->GetInvoiceDate())
		{
			minDate = invoice->GetInvoiceDate();
		}

		if (maxDate.IsNull() || maxDate < invoice->GetInvoiceDate())
		{
			maxDate = invoice->GetInvoiceDate();
		}

	}


	/*
	<tns:ZakupCtrl>
	<tns:LiczbaWierszyZakupow>196</tns:LiczbaWierszyZakupow>
	<tns:PodatekNaliczony>58149.61</tns:PodatekNaliczony>
	</tns:ZakupCtrl>
	*/

	element buy_ctrl(qname(_tns, _U("ZakupCtrl")),
		{
			element(qname(_tns, _U("LiczbaWierszyZakupow")),buyIndex),
			element(qname(_tns, _U("PodatekNaliczony")),buyTax.ToString(_decimalFormat))
		});


	/*
	<tns:SprzedazCtrl>
	<tns:LiczbaWierszySprzedazy>124</tns:LiczbaWierszySprzedazy>
	<tns:PodatekNalezny>80377.53</tns:PodatekNalezny>
	</tns:SprzedazCtrl>
	*/

	element sell_ctrl(qname(_tns, _U("SprzedazCtrl")),
		{
			element(qname(_tns, _U("LiczbaWierszySprzedazy")),sellIndex),
			element(qname(_tns, _U("PodatekNalezny")),sellTax.ToString(_decimalFormat))
		});

 
	

	/*
	<tns:Naglowek>
    <tns:KodFormularza kodSystemowy="JPK_VAT (3)" wersjaSchemy="1-1">JPK_VAT</tns:KodFormularza>
    <tns:WariantFormularza>3</tns:WariantFormularza>
    <tns:CelZlozenia>0</tns:CelZlozenia>
    <tns:DataWytworzeniaJPK>2018-05-23T16:14:29.280</tns:DataWytworzeniaJPK>
    <tns:DataOd>2018-04-01</tns:DataOd>
    <tns:DataDo>2018-04-30</tns:DataDo>
    <tns:NazwaSystemu>Comarch ERP Optima 2018.2.1.950</tns:NazwaSystemu>
    </tns:Naglowek>
	*/
  

	element header_element(qname(_tns, _U("Naglowek")),
		{
			element(qname(_tns, _U("KodFormularza")),
				{
					attribute(_U("kodSystemowy"),_U("JPK_VAT (3)")),
					attribute(_U("wersjaSchemy"),_U("1-1")),
				},
				_U("JPK_VAT")
			),
			element(qname(_tns, _U("WariantFormularza")),3),
			element(qname(_tns, _U("CelZlozenia")),0),
			element(qname(_tns, _U("DataWytworzeniaJPK")), DateTime::now().GetDateInFmt(DateTime::YYYYdashMMdashDDTHHcMIcSS)),
			element(qname(_tns, _U("DataOd")),minDate.GetDateInFmt(DateTime::YYYYdashMMdashDD)),
			element(qname(_tns, _U("DataDo")),maxDate.GetDateInFmt(DateTime::YYYYdashMMdashDD)),
			element(qname(_tns, _U("NazwaSystemu")),_U("BK"))
		});

	/*
	<tns:Podmiot1>
    <tns:NIP>8733092918</tns:NIP>
    <tns:PelnaNazwa>F.H.U. DOBEK SPÓŁKA JAWNA DOBEK SEBASTIAN, DOBEK ANDRZEJ</tns:PelnaNazwa>
    </tns:Podmiot1>
	*/

	IFarm* farm = session->GetFarm();

	element company_element(qname(_tns, _U("Podmiot1")),
		{
			element(qname(_tns, _U("NIP")),farm->GetNIP()),
			element(qname(_tns, _U("PelnaNazwa")),farm->GetName())
		});

	element jpk(qname(_tns, _U("JPK")),
		{
			header_element,
			company_element
		});


	if (!buys.empty())
	{
		jpk += buys;
		jpk += buy_ctrl;
	}

	if (!sells.empty())
	{
		jpk += sells;
		jpk += sell_ctrl;
	}
	

	serializer(output).serialize(jpk);
}

std::wstring JpkExporter::GetInvoiceHentFiscalNo(const IInvoiceHent* hent)
{
	
	std::wstring sFiscalNo = hent->GetFiscalNo();
	if (sFiscalNo.empty())
	{
		sFiscalNo = _none;
	}

	return sFiscalNo;
}

std::wstring JpkExporter::GetHentAddress(const IInvoiceHent* hent)
{
	std::wstring hentAddress = hent->GetStreet() + L" " + hent->GetPOBox() + L", " + hent->GetZip() + L" " + hent->GetCity();

	return hentAddress;
}

DateTime JpkExporter::GetTransactionDate(const IInvoice* invoice)
{
	DateTime transactionDate = invoice->GetTransactionDate();
	if (transactionDate.IsNull())
		transactionDate = invoice->GetInvoiceDate();

	return transactionDate;

}

quickxml::element JpkExporter::SerializeBuy(int index,const IBuyInvoice* invoice)
{
	/*
	<tns:ZakupWiersz>
	<tns:LpZakupu>6</tns:LpZakupu>
	<tns:NrDostawcy>BRAK</tns:NrDostawcy>
	<tns:NazwaDostawcy>Mamiak Jan</tns:NazwaDostawcy>
	<tns:AdresDostawcy>Łomnica Zdrój 289, 33-351 Łomnica Zdrój</tns:AdresDostawcy>
	<tns:DowodZakupu>RR 108-B-2018</tns:DowodZakupu>
	<tns:DataZakupu>2018-03-19</tns:DataZakupu>
	<tns:DataWplywu>2018-03-19</tns:DataWplywu>
	<tns:K_43>0.00</tns:K_43>
	<tns:K_44>0.00</tns:K_44>
	<tns:K_45>1168.22</tns:K_45>
	<tns:K_46>81.78</tns:K_46>
	</tns:ZakupWiersz>
	*/

	InvoiceCalculator::CalculationResult calculation = InvoiceCalculator::Calculate(invoice);

	using namespace quickxml;

	const IInvoiceHent* hent = invoice->GetInvoiceHent();
	
	return element(qname(_tns, _U("ZakupWiersz")), 
			{
				element(qname(_tns, _U("LpZakupu")),TextUtils::Format(L"%d", index)),
				element(qname(_tns, _U("NrDostawcy")),GetInvoiceHentFiscalNo(hent)),
				element(qname(_tns, _U("NazwaDostawcy")),hent->GetName()),
				element(qname(_tns, _U("AdresDostawcy")),GetHentAddress(hent)),
				element(qname(_tns, _U("DowodZakupu")),invoice->GetCustomNumber()),
				element(qname(_tns, _U("DataZakupu")),GetTransactionDate(invoice).GetDayDate()),
				element(qname(_tns, _U("DataWplywu")),invoice->GetInvoiceDate().GetDayDate()),
				element(qname(_tns, _U("K_43")),calculation.GetNetValue().ToString(_decimalFormat)),
				element(qname(_tns, _U("K_44")),calculation.GetTaxValue().ToString(_decimalFormat))

			});
	
}

quickxml::element JpkExporter::SerializeSell(int index,const ISellInvoice* invoice)
{
	/*
	<tns:SprzedazWiersz>
	<tns:LpSprzedazy>7</tns:LpSprzedazy>
	<tns:NrKontrahenta>6961089004</tns:NrKontrahenta>
	<tns:NazwaKontrahenta>Przedsiębiorstwo Handlowe "TECHNIK" Czesław Łapawa</tns:NazwaKontrahenta>
	<tns:AdresKontrahenta>ul. gen. Władysława Sikorskiego 62, 63-800 Gostyń</tns:AdresKontrahenta>
	<tns:DowodSprzedazy>81/2018</tns:DowodSprzedazy>
	<tns:DataWystawienia>2018-04-04</tns:DataWystawienia>
	<tns:K_17>17710.19</tns:K_17>
	<tns:K_18>1416.81</tns:K_18>
	</tns:SprzedazWiersz>
	*/

	InvoiceCalculator::CalculationResult calculation = InvoiceCalculator::Calculate(invoice);

	using namespace quickxml;

	const IInvoiceHent* hent = invoice->GetInvoiceHent();

	return element(qname(_tns, _U("SprzedazWiersz")),
		{
			element(qname(_tns, _U("LpSprzedazy")),TextUtils::Format(L"%d", index)),
			element(qname(_tns, _U("NrKontrahenta")),GetInvoiceHentFiscalNo(hent)),
			element(qname(_tns, _U("NazwaKontrahenta")),hent->GetName()),
			element(qname(_tns, _U("AdresKontrahenta")),GetHentAddress(hent)),
			element(qname(_tns, _U("DowodSprzedazy")),invoice->GetCustomNumber()),			
			element(qname(_tns, _U("DataWystawienia")),invoice->GetInvoiceDate().GetDayDate()),
			element(qname(_tns, _U("K_17")),calculation.GetNetValue().ToString(_decimalFormat)),
			element(qname(_tns, _U("K_18")),calculation.GetTaxValue().ToString(_decimalFormat))

		});



	
}