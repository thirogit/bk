#include "stdafx.h"
#include "SubiektEDI.h"
#include <boost/foreach.hpp>
#include "../../../text/InvoiceNoText.h"
#include "../../../text/PayWayString.h"
#include <string/IntString.h>
#include "../../calculator/InvoiceCalculator.h"
#include <string/TextUtils.h>
#include "../../../utils/Product.h"
#include <data\types\countries\Countries.h>
#include "../../../text/CountryNameText.h"
#include "../../../context/XContext.h"
#include <boost/lexical_cast.hpp>

namespace epp
{
	section::section(const std::wstring& name) : section_name(name)
	{
	}


	section& section::operator+=(const section_row& r)
	{
		m_rows.push_back(r);
		return *this;
	}

	void section::print(std::wofstream& output)
	{
		output << L'[' << section_name << L']' << wendlrn;

		auto it = m_rows.begin();
		while (it != m_rows.end())
		{
			it->print(output);
			it++;
		}
		output << wendlrn;
	}
	
	section_row section_row_forwarder::operator|(const wchar_t* str) const
	{
		section_row r;
		r.operator|(str);
		return std::move(r);
	}

	section_row section_row_forwarder::operator|(int i) const
	{
		section_row r;
		r.operator|(i);
		return std::move(r);
	}

	section_row& section_row::operator|(const wchar_t* str)
	{
		if (wcslen(str) > 0)
		{
			m_columns.push_back(L'\"' + std::wstring(str) + L'\"');
		}
		else
		{
			this->operator|(null);
		}
		return *this;
	}

	section_row& section_row::operator|(const std::wstring& str)
	{
		if (!str.empty())
		{
			m_columns.push_back(L'\"' + str + L'\"');
		}
		else
		{
			this->operator|(null);
		}
		return *this;
	}

	section_row& section_row::operator|(int i)
	{
		m_columns.push_back(boost::lexical_cast<std::wstring>(i));
		return *this;
	}

	section_row& section_row::operator|(const DateTime& dt)
	{
		m_columns.push_back(dt.GetDateInFmt(DateTime::YYYYMMDDHHMISS));
		return *this;
	}

	section_row& section_row::operator|(const null_paceholder& ignored)
	{
		m_columns.push_back(std::wstring());
		return *this;
	}

	section_row& section_row::operator|(const Decimal& dec)
	{
		m_columns.push_back(dec.ToString(4));
		return *this;
	}

	void section_row::print(std::wofstream& output)
	{
		auto it = m_columns.begin();
		while (it != m_columns.end())
		{
			output << *it;
			it++;
			if (it != m_columns.end())
			{
				output << L',';
			}
		}
		output << wendlrn;

	}

	section_row operator|(const section_row_forwarder& fwd, const std::wstring& str)
	{
		section_row r;
		r.operator|(str);
		return std::move(r);
	}



	
	info_section::info_section() : section(L"INFO")
	{
	}


	
	invoice_header::invoice_header() : section(L"NAGLOWEK")
	{

	}

	invoice_header& invoice_header::operator&=(const section_row& r)
	{
		m_rows.push_back(r);
		return *this;
	}	
	

	content::content() : section(L"ZAWARTOSC")
	{
	}

	void content::print(std::wofstream& output)
	{
		section::print(output);
	};


	content_header::content_header(const std::wstring& contentname) : m_contentName(contentname), section(L"NAGLOWEK")
	{
		m_rows.push_back(row | m_contentName);
	}

}



std::wstring SubiektEDI::GetExporterName()
{
	return L"EDI++";
}

std::wstring SubiektEDI::GetFileExtension()
{
	return L"epp";
}

std::string SubiektEDI::GetDefaultEncoding()
{
	return "windows1250";
}

void SubiektEDI::Export(IInvoiceRange invoices,
									 InvoiceExporterOptions& options,
									 std::wofstream& output,
									 SeasonSession* session,
									 SimpleProgress *progress)
{




	IFarm* farm = session->GetFarm();
	XContext* context = session->GetContext();
	ISimpleUser* user = context->GetUser();
	Country  country = farm->GetFarmNo().GetCountry();


	DateTime minDate, maxDate;

	BOOST_FOREACH(IInvoice* invoice, invoices)
	{
		if (minDate.IsNull() || minDate > invoice->GetInvoiceDate())
		{
			minDate = invoice->GetInvoiceDate();
		}

		if (maxDate.IsNull() || maxDate < invoice->GetInvoiceDate())
		{
			maxDate = invoice->GetInvoiceDate();
		}

	}


	using namespace epp;
			
	auto type = 
	choose<std::wstring,InvoiceType, InvoiceDirection>()
		.when(InvoiceType_Lump, InvoiceDirection_Buy).give(L"FR")
		.when(InvoiceType_Regular, InvoiceDirection_Buy).give(L"FZ")
		.when(InvoiceType_Regular, InvoiceDirection_Sell).give(L"FS");
		
	auto mnemonic =
		choose<std::wstring, InvoiceType>()
		.when(InvoiceType_Lump).give(L"RR")
		.when(InvoiceType_Regular).give(L"VAT");

	auto prefix =
		choose<std::wstring, InvoiceType>()
		.when(InvoiceType_Lump).give(L"RR")
		.when(InvoiceType_Regular).give(L"");


	auto payway =
		choose<int,InvoiceDirection,PayWay>()
		.when(InvoiceDirection_Buy, PayWay_Cash).give(4)		
		.when(InvoiceDirection_Sell, PayWay_Transfer).give(3);


	info_section info;
	info += row | L"1.05.3" | 3 | 1250 | Product().NameAndVersion() | farm->GetAlias() | farm->GetAlias() | farm->GetName() | farm->GetCity() | farm->GetZipCode() | farm->GetStreet() + L" " + farm->GetPOBox() | farm->GetNIP() | L"MAG" | null | null | null | 1 | minDate | maxDate | user->GetLogin() | DateTime::now() | CountryNameText(country).ToString() | country.GetCountryCode2A() | country.GetCountryCode2A() + farm->GetNIP() | country.IsCountryInEU();
	info.print(output);
	
	
	std::set<uint32_t> hentIds;

	BOOST_FOREACH(IInvoice* invoice, invoices)
	{
		InvoiceCalculator::CalculationResult calculation = InvoiceCalculator::Calculate(invoice);
		const IInvoiceHent* invoiceHent = invoice->GetInvoiceHent();
		uint32_t hentId = invoiceHent->GetParentHentId();
		hentIds.insert(hentId);
			
		HentClientObject* hentCO = session->GetHent(hentId);
		IHent* hhent = hentCO->Object();
		Country hentsCountry = invoiceHent->GetHentNo().GetCountry();
		int payDueDays = invoice->GetPayDueDays();
		
		invoice_header invoicehdr;
		invoicehdr &= row | type(invoice->GetInvoiceType(), invoice->GetInvoiceDirection()) | 1 | 0 | invoice->GetId() | prefix(invoice->GetInvoiceType()) + L" " + invoice->GetCustomNumber() | null | mnemonic(invoice->GetInvoiceType()) + L" " + invoice->GetCustomNumber() | null | null | null | null |
			invoiceHent->GetHentNo().ToString() | invoiceHent->GetName() | invoiceHent->GetName() | invoiceHent->GetCity() | invoiceHent->GetZip() | hhent->GetStreet() + L" " + hhent->GetPOBox() | hhent->GetFiscalNo() | null | null | invoice->GetTransactionPlace() |
			invoice->GetInvoiceDate() | invoice->GetTransactionDate() | null | invoice->GetCowQty() | 1 | null | calculation.GetNetValue() | calculation.GetTaxValue() | calculation.GetGrossValue() | calculation.GetNetValue() |
			null | 0_d | PayWayString(invoice->GetPayWay()).ToString() | invoice->GetInvoiceDate().AddDays(payDueDays > 0 ? (payDueDays - 1) : payDueDays) | 0_d | calculation.GetGrossValue() | 0 | 0 | 0 | payway(invoice->GetInvoiceDirection(), invoice->GetPayWay()) | null | null | null |
			0_d | 0_d | L"PLN" | 1_d | null | null | null | null | 0 | 0 | 0 | null | 0_d | null | 0_d | CountryNameText(hentsCountry).ToString() | hentsCountry.GetCountryCode2A() | hentsCountry.IsCountryInEU();
			
			;

		content invoicebody;
			
		int index = 1;
		PtrEnumerator<ICowInvoiceEntry> enumCows = invoice->EnumEntries();
		while (enumCows.hasNext())
		{
			ICowInvoiceEntry* entry = *enumCows;

			Decimal weight = entry->GetWeight();
			Decimal vatrate = invoice->GetVATRate();
			Decimal net = entry->GetPrice();
			Decimal tax = net * vatrate;
			Decimal gross = net + tax;
			Decimal perkgnet = net / weight;
			Decimal perkggross = gross / weight;

			invoicebody += row | index | 1 | L"BYDŁO" | 1 | 0 | 0 | 1 | 0_d | 0_d | L"kg" | weight | weight | 0_d | perkgnet | perkggross | vatrate*100_d | net | tax | gross | gross | null | null;

			enumCows.advance();
			index++;
		}
		
		invoicehdr.print(output);
		invoicebody.print(output);
	}

	content hents;


	auto henttype =
		choose<int, HentType>()
		.when(HentType_Company).give(0)
		.when(HentType_Individual).give(4)
		;


	BOOST_FOREACH(uint32_t hentId, hentIds)
	{

		HentClientObject* hentCO = session->GetHent(hentId);
		IHent* hhent = hentCO->Object();
		Country hentsCountry = hhent->GetHentNo().GetCountry();

		auto symbol =
			choose<std::wstring, HentType>()
			.when(HentType_Company).give(hhent->GetAlias())
			.when(HentType_Individual).give(hhent->GetHentNo().ToString())
			;
		
		hents += row | henttype(hhent->GetHentType()) | symbol(hhent->GetHentType()) | hhent->GetName() | hhent->GetName() | hhent->GetCity() | hhent->GetZip() | hhent->GetStreet() + L" " + hhent->GetPOBox() |
			hhent->GetFiscalNo() | hhent->GetREGON() | hhent->GetPhoneNo() | null | null | hhent->GetEmailAddress()
			| null | null | null | null | null | null | null | null | null | null | null | null | hhent->GetBankName()
			| hhent->GetAccountNo().GetNumber() | CountryNameText(hentsCountry).ToString() | hentsCountry.GetCountryCode2A() | hentsCountry.IsCountryInEU();
	}

	
	content_header(L"KONTRAHENCI").print(output);

	hents.print(output);

}


