#include "stdafx.h"
#include "SmallBusinessSymfonia.h"
#include <boost/foreach.hpp>
#include "../../../text/InvoiceNoText.h"
#include "../../../text/PayWayString.h"
#include <string/IntString.h>
#include "../../calculator/InvoiceCalculator.h"
#include <string/TextUtils.h>


std::wstring SmallBusinessSymfonia::GetExporterName()
{
	return L"Symfonia Format 3.0 Mała Księgowość";
}

std::wstring SmallBusinessSymfonia::GetFileExtension()
{
	return L"txt";
}

std::string SmallBusinessSymfonia::GetDefaultEncoding()
{
	return "windows1250";
}


std::wstring SmallBusinessSymfonia::FormatFiscalNo(const std::wstring& fiscalNo)
{
	if (fiscalNo.empty() || TextUtils::IsAllWhitespace(fiscalNo))
		return L"";

	std::wstring formatedFiscalNo = fiscalNo;

	formatedFiscalNo.insert(3, L"-");
	formatedFiscalNo.insert(7, L"-");
	formatedFiscalNo.insert(10, L"-");

	return formatedFiscalNo;
}

void SmallBusinessSymfonia::Export(IInvoiceRange invoices,
									 InvoiceExporterOptions& options,
									 std::wofstream& output,
									 SeasonSession* session,
									 SimpleProgress *progress)
{
	using namespace sagejson;

	std::list<object> objects;
	int companyId = 1;
	const IFarm* company = session->GetFarm();

	object info(L"INFO", {
							object(L"Nazwa programu", L"Krowka"),
							object(L"Wersja_programu", L"2016"),
							object(L"Wersja szablonu", L"3.1"),
							object(L"Kontrahent",
												{
													object(L"id", companyId),
													object(L"kod", company->GetAlias()),
													object(L"nazwa", company->GetName()),
													object(L"miejscowosc", company->GetCity()),
													object(L"ulica", company->GetStreet()),
													object(L"dom", company->GetPOBox()),
													object(L"lokal", L""),
													object(L"kodpocz", company->GetZipCode()),
													object(L"rejon", L""),
													object(L"nip", FormatFiscalNo(company->GetNIP())),
													object(L"tel1", L""),
													object(L"tel2", L""),
													object(L"fax", L""),
													object(L"email", L""),
													object(L"www", L"")
												})

						}
	);

	objects.push_back(info);


	std::list<object> invoiceHents;
	std::list<object> invoiceObjects;
	std::set<uint32_t> exportedHents;

	BOOST_FOREACH(IInvoice* invoice,invoices)
	{
		std::wstring invoiceNo = invoice->GetCustomNumber();
		if (invoiceNo.empty())
			invoiceNo = InvoiceNoText(invoice).ToString();

		Decimal vatRate = invoice->GetVATRate();
		Decimal vatRate100 = vatRate*Decimal(100L);

		DecimalFormat vatRateFmtr(1, L'.');

		InvoiceCalculator::CalculationResult calcResult = InvoiceCalculator::Calculate(invoice);

		Decimal totalNet = calcResult.GetNetValue();
		Decimal totalTax = calcResult.GetTaxValue();
		Decimal totalGross = calcResult.GetGrossValue();
		Decimal totalWeight = calcResult.GetWeight();

		const IInvoiceHent* invoiceHent = invoice->GetInvoiceHent();

		auto exportedHentIt = exportedHents.find(invoiceHent->GetParentHentId());
		if (exportedHentIt == exportedHents.end())
		{
			object invoiceHentObj(L"Kontrahent",
			{
				object(L"id", (int)invoiceHent->GetParentHentId()),
				object(L"kod", invoiceHent->GetHentNo().GetFarmNo().ToString()),
				object(L"nazwa", invoiceHent->GetName()),
				object(L"miejscowosc", invoiceHent->GetCity()),
				object(L"ulica", invoiceHent->GetStreet()),
				object(L"dom", invoiceHent->GetPOBox()),
				object(L"lokal", L""),
				object(L"kodpocz", invoiceHent->GetZip()),
				object(L"rejon", L""),
				object(L"nip", FormatFiscalNo(invoiceHent->GetFiscalNo())),
				object(L"pesel", invoiceHent->GetPESEL()),
				object(L"bkonto", invoiceHent->GetAccountNo().GetNumber()),
				object(L"tel1", L""),
				object(L"tel2", L""),
				object(L"fax", L""),
				object(L"email", L""),
				object(L"www", L""),
				object(L"NazwaRodzaju", L"Kontrahenci"),
				object(L"NazwaKatalogu", L"\\@Kontrahenci")
			});

			invoiceHents.push_back(invoiceHentObj);
		}
		

		std::wstring invoiceDesc = IntString(invoice->GetCowQty()) + L" sztuki bydła na łączną wagę " + totalWeight.ToString(3) + L"kg";
		DateTime invoiceDt = invoice->GetInvoiceDate();
		DateTime transactionDt = invoice->GetTransactionDate();
		if (transactionDt.IsNull())
		{
			transactionDt = invoiceDt;
		}

		object invoiceObj(L"Dokument",
		{
			object(L"rodzaj_dok", L"zakupu"),
			object(L"symbol FK", L"FVZ"),
			object(L"obsluguj jak", L"FVZ"),
			object(L"NazwaKor", L"[FVZ]"),
			object(L"typ_dk", L"FVZ"),
			object(L"seria", L"sFVZ"),
			object(L"nazwa", L"Faktura zakupu"),
			object(L"kod", invoiceNo),
			object(L"kod_obcy", invoiceNo),
			object(L"FK nazwa", invoiceNo),
			object(L"rejestr_vat", L"rZPV"),
			object(L"nazwa_rejestru_vat", L"Zakup VAT"),
			object(L"typ_rejestru_vat", 1),
			object(L"NazwaKatalogu", L"\\@Dokumenty zakupu"),
			object(L"NazwaRodzaju", L"Dokumenty zakupu"),
			object(L"kodMag", L"MAG"),
			object(L"waluta", L""),
			object(L"kurs", 1.0),
			object(L"kursDoch", 1.0),
			object(L"kwota", totalGross),
			object(L"dozaplaty", -totalGross),
			object(L"wdozaplaty", -totalGross),

			object(L"Rozrachunek", 
				{
				object(L"kwota", -totalGross),
				object(L"wkwota", -totalGross),
				object(L"waluta", L""),
			    object(L"kurs", 1.0)
				}),


			object(L"netto", totalNet),
			object(L"vat", -totalTax),
			object(L"walnetto", -totalNet),
			object(L"walbrutto", -totalGross),
			object(L"data", invoiceDt.GetDayDate()),
			object(L"datasp", transactionDt.GetDayDate()),

			object(L"khid", (int)invoiceHent->GetParentHentId()),
			object(L"khkod", invoiceHent->GetHentNo().GetFarmNo().ToString()),
			object(L"khnazwa", invoiceHent->GetName()),
			object(L"khadres", invoiceHent->GetStreet()),
			object(L"khdom", invoiceHent->GetPOBox()),
			object(L"khlokal", L""),
			object(L"khmiasto", invoiceHent->GetCity()),
			object(L"khkodpocz", invoiceHent->GetZip()),
			object(L"khnip", FormatFiscalNo(invoiceHent->GetFiscalNo())),
			object(L"khKrajKod", L"PL"),
			object(L"khKrajNazwa", L"Polska"),
			object(L"odid", companyId),
			object(L"odkod", company->GetAlias()),
			object(L"odnazwa", company->GetName()),
			object(L"odadres", company->GetStreet()),
			object(L"oddom", company->GetPOBox()),
			object(L"odlokal", L""),
			object(L"odmiasto", company->GetCity()),
			object(L"odkodpocz", company->GetZipCode()),
			object(L"rejestr_platnosci", L""),
			object(L"forma_platnosci", PayWayString(invoice->GetPayWay()).ToString()),
			object(L"opis", invoiceDesc),
			object(L"opis FK", invoiceDesc)
		}
		);


		std::list<object> entryObjectDevises;

		PtrEnumerator<ICowInvoiceEntry> entryEnum = invoice->EnumEntries();

		int positionIndex = 0;
		while(entryEnum.hasNext())
		{
			
			ICowInvoiceEntry* entry = *entryEnum;
			CowClientObject* cowCO = session->GetCow(entry->GetCowId());
			Cow* cow = cowCO->Object();

			Decimal netValue = entry->GetPrice();
			Decimal taxValue = vatRate*netValue;
			Decimal grossValue = netValue + taxValue;

			object entryObject(L"Pozycja dokumentu", 
			{
				object(L"kod", L"cielęta"),
				object(L"opis_pozycji", cow->GetCowNo().ToString()),
				object(L"jm", L"szt"),
				object(L"jmwp", L"szt"),
				object(L"ilosc", 1.0),
				object(L"iloscwp", 1.0),
				object(L"cenawal", grossValue),
				object(L"cena", grossValue),
				object(L"wartNetto", netValue),
				object(L"wartVat", taxValue),
				object(L"opisdod", L""),
				object(L"stawkaVAT", vatRate100.ToString(vatRateFmtr)),
				object(L"typ_pozycji", 0)
			});


			entryObjectDevises.push_back(
				object(L"Zapis", 
				{
					object(L"strona", L"WN"),
					object(L"kwota", grossValue),
					object(L"konto", L""),
					object(L"IdDlaRozliczen", L""),
					object(L"opis", (cow->GetCowNo().ToString() + L" - " + invoiceNo)),
					object(L"NumerDok", invoiceNo),
					object(L"Pozycja", positionIndex),
					object(L"ZapisRownolegly", 0)
				}));

			entryObjectDevises.push_back(
				object(L"Zapis", 
				{
					object(L"strona", L"MA"),
					object(L"kwota", netValue),
					object(L"konto", L"10"),
					object(L"IdDlaRozliczen", L""),
					object(L"opis", L""),
					object(L"NumerDok", invoiceNo),
					object(L"Pozycja", positionIndex),
					object(L"ZapisRownolegly", 0)
				}));


			invoiceObj += entryObject;
			positionIndex++;
			entryEnum.advance();

		}

		invoiceObj += entryObjectDevises;

		invoiceObj += object(L"Rejestr", {

			object(L"skrot", L"rZPV"),
			object(L"Nazwa", L"Zakup VAT"),
			object(L"Rodzaj", L""), //2000											
			object(L"Naliczony", 1),
			object(L"ABC", 1),
			object(L"datarej", invoiceDt.GetDayDate()),
			object(L"okres", invoiceDt.GetDayDate()),
			object(L"stawka", vatRate100.ToString(vatRateFmtr)),
			object(L"brutto", totalGross),
			object(L"netto", totalNet),
			object(L"vat", totalTax)
		});

		invoiceObjects.push_back(invoiceObj);
	}


	objects.insert(objects.end(), invoiceHents.begin(), invoiceHents.end());
	objects.insert(objects.end(), invoiceObjects.begin(), invoiceObjects.end());

	auto objIt = objects.begin();
	while (objIt != objects.end())
	{
		WriteObject(output, *objIt);
		objIt++;
	}

}


void SmallBusinessSymfonia::WriteObject(std::wofstream& output, const sagejson::object& o)
{
	std::wstring name = o.name();
	output << name;
	boost::apply_visitor(sagejson::serializer(output), o.value());
}
