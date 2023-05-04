#include "stdafx.h"
#include "InvoiceDocument.h"
#include <boost\foreach.hpp>
#include <string\IntString.h>
#include <arrays\DeepDelete.h>
#include "../../text/CowSexText.h"
#include "../runtime/Label.h"
#include <string/TextUtils.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../options/PrintOptionSerializer.h"
#include <types/Decimal.h>
#include "../../invoice/calculator/InvoiceCalculator.h"


//#######################################################################################

InvoiceOptions::InvoiceOptions(IPrintProfile* profile) : m_priceFormat(2), m_weightFormat(3), m_pricePerKgFormat(5)
{
	if (!profile) return;

	PrintOptionSerializer optionSerializer;

	PrintOption* titleOption = profile->GetOption(L"TITLE");
	if (titleOption)
	{
		m_title = titleOption->get();
	}

	PrintOption* printFmOption = profile->GetOption(L"PRICEFMT");
	if (printFmOption)
	{
		m_priceFormat = optionSerializer.GetDecimalFormatValue(*printFmOption);
	}
	
	PrintOption* weightFmtOption = profile->GetOption(L"WEIGHTFMT");
	if (weightFmtOption)
	{
		m_weightFormat = optionSerializer.GetDecimalFormatValue(*weightFmtOption);
	}

	PrintOption* printPerKgFmtOption = profile->GetOption(L"PRICEPERKGFMT");
	if (printPerKgFmtOption)
	{
		m_pricePerKgFormat = optionSerializer.GetDecimalFormatValue(*printPerKgFmtOption);
	}

	PrintOption* entryFmtOption = profile->GetOption(L"ENTRYFMT");
	if (entryFmtOption)
	{
		m_item = entryFmtOption->get();
	}

	PrintOption* extrasTemplateOption = profile->GetOption(L"EXTRASTEMPLATE");
	if (extrasTemplateOption)
	{
		m_extras = extrasTemplateOption->get();
	}

	PrintOption* goodsCatOption = profile->GetOption(L"GOODSCATEGORY");
	if (goodsCatOption)
	{
		m_goodsCategory = goodsCatOption->get();
	}

	PrintOption* goodsDecl1Option = profile->GetOption(L"GOODSDECLINATION1");
	if (goodsDecl1Option)
	{
		m_goodsDeclination1 = goodsDecl1Option->get();
	}

	PrintOption* goodsDecl2_4Option = profile->GetOption(L"GOODSDECLINATION2_4");
	if (goodsDecl2_4Option)
	{
		m_goodsDeclination2_4 = goodsDecl2_4Option->get();
	}

	PrintOption* goodsDelc4plusOption = profile->GetOption(L"GOODSDECLINATION4+");
	if (goodsDelc4plusOption)
	{
		m_goodsDeclination4plus = goodsDelc4plusOption->get();
	}								
}

const std::wstring& InvoiceOptions::title() const
{
	return m_title;
}

const DecimalFormat& InvoiceOptions::priceFormat() const
{
	return m_priceFormat;
}

const DecimalFormat& InvoiceOptions::weightFormat() const
{
	return m_weightFormat;
}

const DecimalFormat& InvoiceOptions::pricePerKgFormat() const
{
	return m_pricePerKgFormat;
}

const std::wstring& InvoiceOptions::item() const
{
	return m_item;
}

const std::wstring& InvoiceOptions::extras() const
{
	return m_extras;
}

const std::wstring& InvoiceOptions::goodsCategory() const
{
	return m_goodsCategory;
}

const std::wstring& InvoiceOptions::goodsDeclination1() const
{
	return m_goodsDeclination1;
}

const std::wstring& InvoiceOptions::goodsDeclination2_4() const
{
	return m_goodsDeclination2_4;
}

const std::wstring& InvoiceOptions::goodsDeclination4plus() const
{
	return m_goodsDeclination4plus;
}

//#######################################################################################

void HentRecord::CopyFrom(const IFarm* farm)
{
	name = farm->GetName();
	street = farm->GetStreet();
	pobox = farm->GetPOBox();
	city = farm->GetCity();
	zip = farm->GetZipCode();
	number = farm->GetFarmNo().ToString();
	fiscalno = farm->GetNIP();
	personalno.clear();
	personalidno.clear();
	personalidissuedt.clear();
	personalidissuepost.clear();
}

void HentRecord::CopyFrom(const IInvoiceHent* hent)
{
	name = hent->GetName();
	street = hent->GetStreet();
	pobox = hent->GetPOBox();
	city = hent->GetCity();
	zip = hent->GetZip();
	number = hent->GetHentNo().ToString();
	fiscalno = hent->GetFiscalNo();
	accountnumber = hent->GetAccountNo().ToString();
	bankname = hent->GetBankName();

	personalno = hent->GetPESEL();
	personalidno = hent->GetIdNo();
	personalidissuedt = hent->GetIssueDate().GetDayDate();
	personalidissuepost = hent->GetIssuePost();
}


//#######################################################################################
InvoiceTypePredicate::InvoiceTypePredicate(InvoiceType type) : m_type(type)
{
}

bool InvoiceTypePredicate::applies(const InvoiceView* pView)
{
	return pView->GetInvoiceType() == m_type;
}

//#######################################################################################
InvoiceRecord::InvoiceRecord() 
{

}

//#######################################################################################
InvoiceItemRow::InvoiceItemRow() : m_qtyUnit(QtyUnit_None)
{
}

InvoiceItemRow::InvoiceItemRow(const InvoiceItemRow& src)
{
	CopyFrom(src);
}

InvoiceItemRow& InvoiceItemRow::operator=(const InvoiceItemRow& right)
{
	CopyFrom(right);
	return *this;
}

void InvoiceItemRow::CopyFrom(const InvoiceItemRow& src)
{
	m_sDesc = src.m_sDesc;
	m_qtyPiece = src.m_qtyPiece;
	m_qtyWeight = src.m_qtyWeight;
	m_qtyUnit = src.m_qtyUnit;
	m_netTotal = src.m_netTotal;
	m_vatTotal = src.m_vatTotal;
}
	
void InvoiceItemRow::SetDescription(const std::wstring& sDesc)
{
	m_sDesc = sDesc;
}

const std::wstring& InvoiceItemRow::GetDescription() const
{
	return m_sDesc;
}

void InvoiceItemRow::SetQty(const Decimal& weight)
{
	m_qtyWeight = weight;
	m_qtyUnit = QtyUnit_Kilogram;
}

void InvoiceItemRow::SetQty(int pieces)
{
	m_qtyPiece = pieces;
	m_qtyUnit = QtyUnit_Piece;
}

QtyUnit InvoiceItemRow::GetQtyUnit() const
{
	return m_qtyUnit;
}

NullDecimal InvoiceItemRow::GetQtyWeight() const
{
	if (m_qtyUnit == QtyUnit_Kilogram)
		return m_qtyWeight;

	return NullDecimal();
}
NullInt InvoiceItemRow::GetQtyPieces() const
{	
	if (m_qtyUnit == QtyUnit_Piece)
		return m_qtyPiece;

	return NullInt();
}

void InvoiceItemRow::SetNetTotal(const Decimal& total)
{
	m_netTotal = total;
}

const Decimal& InvoiceItemRow::GetNetTotal() const
{
	return m_netTotal;
}

NullDecimal InvoiceItemRow::GetUnitPrice() const
{
	return NullDecimal(GetNetTotal())/GetQty();
}

NullDecimal InvoiceItemRow::GetQty() const
{
	switch(m_qtyUnit)
	{
		case QtyUnit_Piece:
			return NullDecimal((long)m_qtyPiece.ToInt());
		case QtyUnit_Kilogram:
			return m_qtyWeight;
	}

	return NullDecimal();
}

void InvoiceItemRow::SetVatTotal(const Decimal& vatTotal)
{
	m_vatTotal = vatTotal;
}

const Decimal& InvoiceItemRow::GetVatTotal() const
{
	return m_vatTotal;
}
//#######################################################################################

GenericInvoiceDocument::HyperInvoiceDocumentDS::HyperInvoiceDocumentDS(InvoiceRecordRange records, InvoiceOptions* options, GenericInvoiceDocument* parent)
{
	BOOST_FOREACH(const InvoiceRecord* record, records)
	{
		m_dataSources.push_back(new InvoiceDocumentDS(record, options, parent));
	}
}

GenericInvoiceDocument::HyperInvoiceDocumentDS::~HyperInvoiceDocumentDS()
{
	DeepDelete(m_dataSources);
}


int GenericInvoiceDocument::HyperInvoiceDocumentDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* GenericInvoiceDocument::HyperInvoiceDocumentDS::GetDS(int index)
{
	return m_dataSources[index];
}

//#######################################################################################
GenericInvoiceDocument::InvoiceDocumentDS::ItemsBandEvaluator::ItemsBandEvaluator(InvoiceDocumentDS* parent) : m_parent(parent)
{

}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::ItemsBandEvaluator::Footer()
{
	bkprint::Values values;
	DecimalFormat priceFmt = m_parent->m_options->priceFormat();
	DecimalFormat vatRateFmt(2, priceFmt.GetDecimalPoint());

	values.Set(L"VATRATE", (Decimal(100L) * m_parent->m_record->vatRate).ToString(vatRateFmt));
	values.Set(L"TOTALNETVALUE", m_parent->m_record->totalNet.ToString(priceFmt));
	values.Set(L"TOTALVATVALUE", m_parent->m_record->totalVat.ToString(priceFmt));
	values.Set(L"TOTALGROSS", m_parent->m_record->totalGross.ToString(priceFmt));
	return std::move(values);
}

size_t GenericInvoiceDocument::InvoiceDocumentDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_rows.size();
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::ItemsBandEvaluator::At(int index)
{
	InvoiceItemRow row = m_parent->m_rows[index];	
	return std::move(CreateRowValues(index, &row));
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const InvoiceItemRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex+1));
	values.Set(L"GOODSNAME", row->GetDescription());

	values.Set(L"GOODSCATEGORY", m_parent->m_options->goodsCategory());
	values.Set(L"QTY", GetQty(row));	
	values.Set(L"UNIT", GetQtyUnit(row));
	values.Set(L"UNITPRICENETTO", row->GetUnitPrice().ToString(m_parent->m_options->pricePerKgFormat()));
	
	Decimal netTotal = row->GetNetTotal();
	Decimal vatTotal = row->GetVatTotal();
	Decimal grossTotal = netTotal + vatTotal;

	DecimalFormat priceFormat = m_parent->m_options->priceFormat();
	values.Set(L"NETVALUE",  netTotal.ToString(priceFormat));
	values.Set(L"VATVALUE", vatTotal.ToString(priceFormat));
	values.Set(L"GROSSVALUE", grossTotal.ToString(priceFormat));
	return std::move(values);
}

std::wstring GenericInvoiceDocument::InvoiceDocumentDS::ItemsBandEvaluator::GetQty(const InvoiceItemRow *pRow) const
{
	switch (pRow->GetQtyUnit())
	{
	case QtyUnit_Piece:
		return IntString(pRow->GetQtyPieces().ToInt());
	case QtyUnit_Kilogram:
		return pRow->GetQtyWeight().ToString(m_parent->m_options->weightFormat());
	}

	return std::wstring();
}

std::wstring GenericInvoiceDocument::InvoiceDocumentDS::ItemsBandEvaluator::GetQtyUnit(const InvoiceItemRow *pRow) const
{
	return m_parent->m_parent->m_pContext->GetQtyUnitText(pRow->GetQtyUnit());
}

//#######################################################################################

GenericInvoiceDocument::InvoiceDocumentDS::DeductionsBandEvaluator::DeductionsBandEvaluator(InvoiceDocumentDS* parent) : m_parent(parent)
{
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::DeductionsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::DeductionsBandEvaluator::Footer()
{
	bkprint::Values values;
	values.Set(L"TOTALDEDCTEDAMOUNT", m_parent->m_record->totalDeductedAmount.ToString(m_parent->m_options->priceFormat()));
	return std::move(values);
}

size_t GenericInvoiceDocument::InvoiceDocumentDS::DeductionsBandEvaluator::Count() const
{
	return m_parent->m_record->deductions.size();
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::DeductionsBandEvaluator::At(int index)
{
	return std::move(CreateRowValues(index,m_parent->m_record->deductions[index]));
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::DeductionsBandEvaluator::CreateRowValues(size_t nindex, const DeductionItem* row)
{
	bkprint::Values values;
	values.Set(L"INDEX", IntString(nindex + 1));
	values.Set(L"DEDUCTIONTITLE", row->m_reason);
	values.Set(L"DEDUCTIONFRACTION", (row->m_fraction * Decimal(100L)).ToString(2) + L"%");
	values.Set(L"DEDUCTEDAMOUNT", row->m_amount.ToString(m_parent->m_options->priceFormat()));
		
	return std::move(values);
}

//#######################################################################################

GenericInvoiceDocument::InvoiceDocumentDS::InvoiceDocumentDS(const InvoiceRecord* record, InvoiceOptions* options, GenericInvoiceDocument* parent) :
m_parent(parent), m_record(record), m_options(options), m_deductionsBandEvaluator(NULL)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
	if (!m_record->deductions.empty())
	{
		m_deductionsBandEvaluator = new DeductionsBandEvaluator(this);
	}

	m_rows = std::move(parent->GetRowsForInvoiceRecord(record, m_options));

}

GenericInvoiceDocument::InvoiceDocumentDS::~InvoiceDocumentDS()
{
	delete m_itemsBandEvaluator;
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::GetHeaderValues()
{
	bkprint::Values heaaderValues;

	heaaderValues.Set(L"INVOICETITLE", m_options->title());
	heaaderValues.Set(L"INVOICENO", m_record->invoiceNo);
	heaaderValues.Set(L"INVOICEDATE", m_record->invoiceDate.GetDayDate());
	heaaderValues.Set(L"TRANSACTIONDATE", m_record->transactionDate.GetDayDate());
	heaaderValues.Set(L"TRANSACTIONPLACE", m_record->transactionPlace);
		
	heaaderValues.Set(L"SELLERNAME", m_record->seller.name);
	heaaderValues.Set(L"SELLERADDRESS", FormatHentAddress(&m_record->seller));
	heaaderValues.Set(L"SELLERFARMNO", m_record->seller.number);
	heaaderValues.Set(L"SELLERWETNO", L"");
	heaaderValues.Set(L"SELLERFISCALNO", m_record->seller.fiscalno);
	heaaderValues.Set(L"SELLERACCOUNTNO", m_record->seller.accountnumber);
	heaaderValues.Set(L"SELLERBANKNAME", m_record->seller.bankname);
	heaaderValues.Set(L"SELLERPERSONALNO", m_record->seller.personalno);
	heaaderValues.Set(L"PERSONALIDNO", m_record->seller.personalidno);
	heaaderValues.Set(L"PERSONALIDISSUEPOST", m_record->seller.personalidissuepost);
	heaaderValues.Set(L"PERSONALIDISSUEDATE", m_record->seller.personalidissuedt);

	heaaderValues.Set(L"BUYERNAME", m_record->buyer.name);
	heaaderValues.Set(L"BUYERADDRESS", FormatHentAddress(&m_record->buyer));
	heaaderValues.Set(L"BUYERFARMNO", m_record->buyer.number);
	heaaderValues.Set(L"BUYERWETNO", L"");
	heaaderValues.Set(L"BUYERFISCALNO", m_record->buyer.fiscalno);
	heaaderValues.Set(L"BUYERACCOUNTNO", m_record->buyer.accountnumber);
	heaaderValues.Set(L"BUYERBANKNAME", m_record->buyer.bankname);

	return std::move(heaaderValues);
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::GetFooterValues()
{
	const wchar_t* notApplicable = L"---";

	bkprint::Values values;

	PayWay payway = m_record->payway;

	std::wstring bankname, accountno;
	switch (payway)
	{
		case PayWay_Cash:
			bankname = notApplicable;
			accountno = notApplicable;
		break;
		case PayWay_Transfer:
			bankname = m_record->seller.bankname;
			accountno = m_record->seller.accountnumber;
		break;
	}
	values.Set(L"TOTALOWED", m_record->totalGrossAfterDeductions.ToString(m_options->priceFormat()));
	values.Set(L"PAYDUEDAYS", IntString(m_record->payduedays));
	values.Set(L"PAYWAY", m_parent->m_pContext->GetPayWayText(m_record->payway));
	values.Set(L"BANKNAME", bankname);
	values.Set(L"ACCOUNTNO", accountno);

	std::wstring extras = m_record->extras;
	if (extras.empty())
	{
		extras = m_options->extras();
	}

	values.Set(L"EXTRAS", extras);
	values.Set(L"TOTALOWEDTEXT", m_parent->m_pContext->GetPriceText(m_record->totalGrossAfterDeductions, m_parent->m_pContext->GetCurrencySymbol()));
	

	return std::move(values);
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"INVOICENO", m_record->invoiceNo);
	return std::move(values);
}

bkprint::Values GenericInvoiceDocument::InvoiceDocumentDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* GenericInvoiceDocument::InvoiceDocumentDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	if (L"deductions" == bandName)
		return m_deductionsBandEvaluator;

	return NULL;
}



std::wstring GenericInvoiceDocument::InvoiceDocumentDS::FormatHentAddress(const HentRecord* hent)
{
	return TextUtils::Format(L"%s %s\n%s %s", hent->street, hent->pobox, hent->zip, hent->city);
}

//#######################################################################################

GenericInvoiceDocument::GenericInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface) 
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

bool GenericInvoiceDocument::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if (m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

void GenericInvoiceDocument::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

bkprint::PrintOut GenericInvoiceDocument::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;	
	InvoiceOptions options(profile);
	HyperInvoiceDocumentDS ds(m_pInputInterface->GetRecords(), &options, this);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));

	return std::move(printOut);
}

GenericInvoiceDocument::~GenericInvoiceDocument()
{
}

//#################################################################################################

InvoiceInputInterface::~InvoiceInputInterface()
{
	DeleteRecords();
}

InvoiceRecordRange InvoiceInputInterface::GetRecords()
{
	return InvoiceRecordRange(m_records);
}

bool InvoiceInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void InvoiceInputInterface::Feed(SeasonSession* pSession, const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession, range);
}

void InvoiceInputInterface::AddRecord(InvoiceRecord* record)
{
	m_records.push_back(record);
}

void InvoiceInputInterface::DeleteRecords()
{
	BOOST_FOREACH(InvoiceRecord* record, m_records)
	{
		DeepDelete(record->items);
		DeepDelete(record->deductions);
		delete record;
	}
	m_records.clear();
}

//#################################################################################################

void BuyInvoiceInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t invoiceId, range)
	{
		BuyInvoiceClientObject* pBuyInvoiceCO = pSession->GetBuyInvoice(invoiceId);
		BuyInvoice* pBuyInvoice = pBuyInvoiceCO->Object();
		
		InvoiceRecord* record = new InvoiceRecord();

		record->buyer.CopyFrom(pSession->GetFarm());

		PtrEnumerator<IBankAccount> accountEnum = pSession->EnumBankAccounts();
		if (accountEnum.hasNext())
		{
			IBankAccount* account = *accountEnum;
			record->buyer.accountnumber = account->GetAccountNo().ToString();
			record->buyer.bankname = account->GetBankName();
		}	


		record->seller.CopyFrom(pBuyInvoice->GetInvoiceHent());

		record->extras = pBuyInvoice->GetExtras();
		record->invoiceDate = pBuyInvoice->GetInvoiceDate();

		record->invoiceNo = pBuyInvoice->GetCustomNumber();
		if (record->invoiceNo.empty())
			record->invoiceNo = IntString(pBuyInvoice->GetInvoiceNo());

		record->transactionDate = pBuyInvoice->GetTransactionDate();
		record->transactionPlace = pBuyInvoice->GetTransactionPlace();
		record->payway = pBuyInvoice->GetPayWay();
		record->payduedays = pBuyInvoice->GetPayDueDays();

		InvoiceCalculator::CalculationResult calculation = InvoiceCalculator::Calculate(pBuyInvoice);

		record->totalNet = calculation.GetNetValue();
		record->totalVat = calculation.GetTaxValue();
		record->totalGross = calculation.GetGrossValue();
		record->totalDeductedAmount = calculation.GetDeductedAmount();
		record->totalGrossAfterDeductions = calculation.GetGrossAfterDeductionsValue();
		record->vatRate = pBuyInvoice->GetVATRate();

		PtrEnumerator<ICowInvoiceEntry> entries = pBuyInvoice->EnumEntries();
		while (entries.hasNext())
		{
			ICowInvoiceEntry* entry = *entries;
			InvoiceItem* item = new InvoiceItem();
			ICow* cow = pSession->GetCow(entry->GetCowId())->Object();
			IClass* cowClass = pSession->GetClass(entry->GetClassId());
			item->cowNo = cow->GetCowNo();

			item->cowClass.code = cowClass->GetClassCode();
			item->cowClass.name = cowClass->GetClassNm();

			item->sex = cow->GetSex();
			item->weight = entry->GetWeight();
			item->netValue = entry->GetPrice();
			item->taxValue = pBuyInvoice->GetVATRate() * item->netValue;			

			entries.advance();
			record->items.push_back(item);
		}

		PtrEnumerator<IInvoiceDeduction> deductions = pBuyInvoice->EnumDeductions();
		while (deductions.hasNext())
		{
			IInvoiceDeduction* deduction = *deductions;
			DeductionItem* deductionItem = new DeductionItem();

			if (deduction->GetIsEnabled())
			{
				deductionItem->m_code = deduction->GetCode();
				deductionItem->m_fraction = deduction->GetFraction();
				deductionItem->m_reason = deduction->GetReason();
				deductionItem->m_amount = (calculation.GetNetValue() * deduction->GetFraction());
				record->deductions.push_back(deductionItem);
			}
			deductions.advance();
		}
		AddRecord(record);
	}
}

//#################################################################################################

void SellInvoiceInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t invoiceId, range)
	{
		SellInvoiceClientObject* pInvoiceCO = pSession->GetSellInvoice(invoiceId);
		SellInvoice* pInvoice = pInvoiceCO->Object();

		InvoiceRecord* record = new InvoiceRecord();

		record->seller.CopyFrom(pSession->GetFarm());


		PtrEnumerator<IBankAccount> accountEnum = pSession->EnumBankAccounts();
		if (accountEnum.hasNext())
		{
			IBankAccount* account = *accountEnum;
			record->seller.accountnumber = account->GetAccountNo().ToString();
			record->seller.bankname = account->GetBankName();
		}

		record->buyer.CopyFrom(pInvoice->GetInvoiceHent());

		record->extras = pInvoice->GetExtras();
		record->invoiceDate = pInvoice->GetInvoiceDate();

		record->invoiceNo = pInvoice->GetCustomNumber();
		if (record->invoiceNo.empty())
			record->invoiceNo = IntString(pInvoice->GetInvoiceNo());

		record->transactionDate = pInvoice->GetTransactionDate();
		record->transactionPlace = pInvoice->GetTransactionPlace();
		record->payway = pInvoice->GetPayWay();
		record->payduedays = pInvoice->GetPayDueDays();

		InvoiceCalculator::CalculationResult calculation = InvoiceCalculator::Calculate(pInvoice);

		record->totalNet = calculation.GetNetValue();
		record->totalVat = calculation.GetTaxValue();
		record->totalGross = calculation.GetGrossValue();
		record->totalDeductedAmount = calculation.GetDeductedAmount();
		record->totalGrossAfterDeductions = calculation.GetGrossAfterDeductionsValue();
		record->vatRate = pInvoice->GetVATRate();

		PtrEnumerator<ICowInvoiceEntry> entries = pInvoice->EnumEntries();
		while (entries.hasNext())
		{
			ICowInvoiceEntry* entry = *entries;
			InvoiceItem* item = new InvoiceItem();
			ICow* cow = pSession->GetCow(entry->GetCowId())->Object();
			IClass* cowClass = pSession->GetClass(entry->GetClassId());
			item->cowNo = cow->GetCowNo();
			item->cowClass.code = cowClass->GetClassCode();
			item->cowClass.name = cowClass->GetClassNm();
			item->sex = cow->GetSex();
			item->weight = entry->GetWeight();
			item->netValue = entry->GetPrice();
			item->taxValue = pInvoice->GetVATRate() * item->netValue;

			entries.advance();
			record->items.push_back(item);
		}

		PtrEnumerator<IInvoiceDeduction> deductions = pInvoice->EnumDeductions();
		while (deductions.hasNext())
		{
			IInvoiceDeduction* deduction = *deductions;
			DeductionItem* deductionItem = new DeductionItem();

			if (deduction->GetIsEnabled())
			{
				deductionItem->m_code = deduction->GetCode();
				deductionItem->m_fraction = deduction->GetFraction();
				deductionItem->m_reason = deduction->GetReason();
				deductionItem->m_amount = (calculation.GetNetValue() * deduction->GetFraction());
				record->deductions.push_back(deductionItem);
			}
			deductions.advance();
		}
		AddRecord(record);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ProductNumeralDeclination::ProductNumeralDeclination(InvoiceOptions* options) : m_options(options)
{
}

std::wstring ProductNumeralDeclination::Declinate(int qty) const
{
	std::wstring declination[3] =
	{
		m_options->goodsDeclination1(),
		m_options->goodsDeclination2_4(),
		m_options->goodsDeclination4plus()
	};

	uint32_t a, bc, b, c, abc;
	uint32_t index = 0;

	abc = qty % 1000;
	a = abc / 100;
	bc = abc % 100;
	b = bc / 10;
	c = bc % 10;

	if (abc != 1)
	{
		if (b != 1 && c >= 2 && c <= 4) index = 1;
		else index = 2;
	}
	return declination[index];
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

InvoiceEntryVariables::InvoiceEntryVariables(InvoiceOptions* options, DocumentContext* context) : m_options(options), m_context(context), m_declination(options)
{

}

MustacheVariable InvoiceEntryVariables::ProductNameVariable(int qty) const
{
	return MustacheVariable(L"TOWAR", m_declination.Declinate(qty));
}

MustacheVariable InvoiceEntryVariables::ProductQtyVariable(int qty) const
{
	return MustacheVariable(L"ILOSC", IntString(qty));
}

MustacheVariable InvoiceEntryVariables::CowNoVariable(const CowNo& cowNo) const
{
	return MustacheVariable(L"KOLCZYK", cowNo.ToString());
}

MustacheVariable InvoiceEntryVariables::CowSexVariable(const CowSex& sex) const
{
	return MustacheVariable(L"PLEC", m_context->GetCowSexText(sex));
}

MustacheVariable InvoiceEntryVariables::CowClassCodeVariable(const std::wstring& classCd) const
{
	return MustacheVariable(L"KODKLASY", classCd);
}

MustacheVariable InvoiceEntryVariables::CowClassNameVariable(const std::wstring& classNm) const
{
	return MustacheVariable(L"NAZWAKLASY", classNm);
}

MustacheVariable InvoiceEntryVariables::TotalWeight(const Decimal& weight) const
{
	DecimalFormat weightFmt = m_context->GetWeightFormat();
	return MustacheVariable(L"WAGA", weight.ToString(weightFmt));
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


DetailedPieceInvoiceDocument::DetailedPieceInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface) : GenericInvoiceDocument(templ, inputInterface)
{
}

std::vector<InvoiceItemRow> DetailedPieceInvoiceDocument::GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options)
{
	InvoiceEntryVariables entryVariables(options, m_pContext);
	Mustache mustache = Mustache::Parse(options->item());

	std::vector<InvoiceItemRow> result;
	BOOST_FOREACH(const InvoiceItem* item, record->items)
	{
		InvoiceItemRow row;

		MustacheVariables variables;
		variables.Put(entryVariables.ProductNameVariable(1));
		variables.Put(entryVariables.ProductQtyVariable(1));
		variables.Put(entryVariables.CowClassCodeVariable(item->cowClass.code));
		variables.Put(entryVariables.CowClassNameVariable(item->cowClass.name));
		variables.Put(entryVariables.CowNoVariable(item->cowNo));
		variables.Put(entryVariables.CowSexVariable(item->sex));

		std::wstring entryDescription = mustache.Format(variables);
		if (entryDescription.empty())
		{
			entryDescription = item->cowNo.ToString() + L", " + CowSexText(item->sex).ToString();
		}

		row.SetDescription(entryDescription);
		row.SetQty(1);
		row.SetNetTotal(item->netValue);
		row.SetVatTotal(item->taxValue);
		result.push_back(row);
	}
	return std::move(result);
}

//#################################################################################################


DetailedWeightInvoiceDocument::DetailedWeightInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface) : GenericInvoiceDocument(templ, inputInterface)
{
}

std::vector<InvoiceItemRow> DetailedWeightInvoiceDocument::GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options)
{
	InvoiceEntryVariables entryVariables(options, m_pContext);
	Mustache mustache = Mustache::Parse(options->item());

	std::vector<InvoiceItemRow> result;
	BOOST_FOREACH(const InvoiceItem* item, record->items)
	{
		InvoiceItemRow row;

		MustacheVariables variables;
		variables.Put(entryVariables.ProductNameVariable(1));
		variables.Put(entryVariables.ProductQtyVariable(1));
		variables.Put(entryVariables.CowClassCodeVariable(item->cowClass.code));
		variables.Put(entryVariables.CowClassNameVariable(item->cowClass.name));
		variables.Put(entryVariables.CowNoVariable(item->cowNo));
		variables.Put(entryVariables.CowSexVariable(item->sex));

		std::wstring entryDescription = mustache.Format(variables);
		if (entryDescription.empty())
		{
			entryDescription = item->cowNo.ToString() + L", " + CowSexText(item->sex).ToString();
		}

		row.SetDescription(entryDescription);
		row.SetQty(item->weight);
		row.SetNetTotal(item->netValue);
		row.SetVatTotal(item->taxValue);
		result.push_back(row);
	}
	return std::move(result);
}

//#################################################################################################


std::map<std::wstring, InvoiceItemsGroupByClass*> groupByClassCd(const InvoiceRecord* record)
{
	std::map<std::wstring, InvoiceItemsGroupByClass*> class2qty;
	BOOST_FOREACH(const InvoiceItem* item, record->items)
	{
		InvoiceItemsGroupByClass* group;
		auto groupIt = class2qty.find(item->cowClass.code);
		if (groupIt == class2qty.end())
		{
			group = new InvoiceItemsGroupByClass();
			group->qty = 0;
			group->cowClass = item->cowClass;
			class2qty[item->cowClass.code] = group;
		}
		else
		{
			group = groupIt->second;
		}

		group->qty++;
		group->weight += item->weight;
		group->price += item->netValue;

	}
	return std::move(class2qty);
}


ClassPieceInvoiceDocument::ClassPieceInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface) : GenericInvoiceDocument(templ, inputInterface)
{
	
}

std::vector<InvoiceItemRow> ClassPieceInvoiceDocument::GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options)
{
	std::map<std::wstring, InvoiceItemsGroupByClass*> class2qty = std::move(groupByClassCd(record));
	
	InvoiceEntryVariables entryVariables(options, m_pContext);
	Mustache mustache = Mustache::Parse(options->item());
	std::vector<InvoiceItemRow> result;
	auto groupIt = class2qty.begin();
	for (; groupIt != class2qty.end();groupIt++)
	{ 
		InvoiceItemsGroupByClass* group = groupIt->second;
		InvoiceItemRow row;
		MustacheVariables variables;
		variables.Put(entryVariables.ProductNameVariable(group->qty));
		variables.Put(entryVariables.ProductQtyVariable(group->qty));
		variables.Put(entryVariables.CowClassCodeVariable(groupIt->second->cowClass.code));
		variables.Put(entryVariables.CowClassNameVariable(groupIt->second->cowClass.name));

		std::wstring entryDescription = mustache.Format(variables);
		if (entryDescription.empty())
		{			
			entryDescription = entryVariables.ProductNameVariable(1).GetValue() + L" " + groupIt->first;
		}

		row.SetDescription(entryDescription);
		row.SetQty(group->qty);
		row.SetNetTotal(group->price);
		row.SetVatTotal(record->vatRate * group->price);

		result.push_back(row);
	}

	DeepDelete(class2qty);
	return std::move(result);

}

//#################################################################################################

ClassWeightInvoiceDocument::ClassWeightInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface) : GenericInvoiceDocument(templ, inputInterface)
{

}

std::vector<InvoiceItemRow> ClassWeightInvoiceDocument::GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options)
{
	std::map<std::wstring, InvoiceItemsGroupByClass*> class2qty = std::move(groupByClassCd(record));

	InvoiceEntryVariables entryVariables(options, m_pContext);
	Mustache mustache = Mustache::Parse(options->item());
	std::vector<InvoiceItemRow> result;
	auto groupIt = class2qty.begin();
	for (; groupIt != class2qty.end(); groupIt++)
	{
		InvoiceItemsGroupByClass* group = groupIt->second;
		InvoiceItemRow row;
		MustacheVariables variables;
		variables.Put(entryVariables.ProductNameVariable(group->qty));
		variables.Put(entryVariables.ProductQtyVariable(group->qty));
		variables.Put(entryVariables.CowClassCodeVariable(groupIt->second->cowClass.code));
		variables.Put(entryVariables.CowClassNameVariable(groupIt->second->cowClass.name));

		std::wstring entryDescription = mustache.Format(variables);
		if (entryDescription.empty())
		{
			entryDescription = IntString(group->qty) + L" " + entryVariables.ProductNameVariable(group->qty).GetValue() + L" " + groupIt->first;
		}

		row.SetDescription(entryDescription);
		row.SetQty(group->weight);
		row.SetNetTotal(group->price);
		row.SetVatTotal(record->vatRate * group->price);

		result.push_back(row);
	}
	DeepDelete(class2qty);
	return std::move(result);

}

//#################################################################################################

CompactPieceInvoiceDocument::CompactPieceInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface) : GenericInvoiceDocument(templ, inputInterface)
{
}

std::vector<InvoiceItemRow> CompactPieceInvoiceDocument::GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options)
{
	int qty = record->items.size();
	InvoiceItemRow row;
	InvoiceEntryVariables entryVariables(options, m_pContext);
	Mustache mustache = Mustache::Parse(options->item());
	MustacheVariables variables;
	variables.Put(entryVariables.ProductNameVariable(qty));
	variables.Put(entryVariables.ProductQtyVariable(qty));
	
	std::wstring entryDescription = mustache.Format(variables);
	if (entryDescription.empty())
	{
		entryDescription = entryVariables.ProductNameVariable(1).GetValue();
	}

	row.SetDescription(entryDescription);
	row.SetQty(qty);
	row.SetNetTotal(record->totalNet);
	row.SetVatTotal(record->totalVat);
	std::vector<InvoiceItemRow> result;
	result.push_back(row);
	return std::move(result);
}

//#################################################################################################

CompactWeightInvoiceDocument::CompactWeightInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface) : GenericInvoiceDocument(templ, inputInterface)
{
}

std::vector<InvoiceItemRow> CompactWeightInvoiceDocument::GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options)
{
	Decimal totalWeight;
	BOOST_FOREACH(const InvoiceItem* item, record->items)
	{
		totalWeight += item->weight;
	}

	int qty = record->items.size();
	InvoiceItemRow row;
	InvoiceEntryVariables entryVariables(options, m_pContext);
	Mustache mustache = Mustache::Parse(options->item());
	MustacheVariables variables;
	variables.Put(entryVariables.ProductNameVariable(qty));
	variables.Put(entryVariables.ProductQtyVariable(qty));

	std::wstring entryDescription = mustache.Format(variables);
	if (entryDescription.empty())
	{
		entryDescription = IntString(qty) + L" " + entryVariables.ProductNameVariable(qty).GetValue();
	}

	row.SetDescription(entryDescription);
	row.SetQty(totalWeight);
	row.SetNetTotal(record->totalNet);
	row.SetVatTotal(record->totalVat);
	std::vector<InvoiceItemRow> result;
	result.push_back(row);
	return std::move(result);	
}
