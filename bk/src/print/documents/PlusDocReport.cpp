#include "stdafx.h"
#include "PlusDocReport.h"

#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>

#include <string\TextUtils.h>
#include <string\IntString.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../../text/DocNoText.h"


PlusDocReport::PlusDocReportDS::ItemsBandEvaluator::ItemsBandEvaluator(PlusDocReportDS* parent) : m_parent(parent)
{

}

bkprint::Values PlusDocReport::PlusDocReportDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values PlusDocReport::PlusDocReportDS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t PlusDocReport::PlusDocReportDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_parent->m_pInputInterface->RowCount();
}

bkprint::Values PlusDocReport::PlusDocReportDS::ItemsBandEvaluator::At(int index)
{	
	return std::move(CreateRowValues(index, m_parent->m_parent->m_pInputInterface->RowAt(index)));
}

bkprint::Values PlusDocReport::PlusDocReportDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const PlusDocReportRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex + 1));
	values.Set(L"COWNO", row->cowNo.ToString());
	values.Set(L"INDOCNO", row->plusDocNo);
	if (row->buyFromHent.is_initialized())
	{
		values.Set(L"BUYFROMHENT", FormatInvoiceHent(row->buyFromHent.get()));
	}
	
	return std::move(values);
}

std::wstring PlusDocReport::PlusDocReportDS::ItemsBandEvaluator::FormatInvoiceHent(const InvoiceHent& hent)
{
	return TextUtils::FormatA(L"%s, %s\n%s %s,%s %s",
		hent.GetName().c_str(),
		hent.GetHentNo().ToString().c_str(),
		hent.GetStreet().c_str(),
		hent.GetPOBox().c_str(),
		hent.GetZip().c_str(),
		hent.GetCity().c_str());
}

//#######################################################################################

PlusDocReport::PlusDocReportDS::PlusDocReportDS(PlusDocReport* parent, IPrintProfile* profile) : m_parent(parent)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

PlusDocReport::PlusDocReportDS::~PlusDocReportDS()
{
	delete m_itemsBandEvaluator;
}

bkprint::Values PlusDocReport::PlusDocReportDS::GetHeaderValues()
{
	return std::move(bkprint::Values());
}

bkprint::Values PlusDocReport::PlusDocReportDS::GetFooterValues()
{
	return std::move(bkprint::Values());
}

bkprint::Values PlusDocReport::PlusDocReportDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values PlusDocReport::PlusDocReportDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* PlusDocReport::PlusDocReportDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}

//#######################################################################################

PlusDocReport::PlusDocReport(bkprint::Template* templ, PlusDocReportInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

PlusDocReport::~PlusDocReport()
{
}

bool PlusDocReport::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut PlusDocReport::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;

	PlusDocReportDS ds(this, profile);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));	

	return std::move(printOut);

}

void PlusDocReport::SetContext(DocumentContext* context)
{
	m_pContext = context;
}


//#################################################################################################

PlusDocReportInputInterface::~PlusDocReportInputInterface()
{
	DeleteRows();
}

void PlusDocReportInputInterface::DeleteRows()
{
	DeepDelete(m_rows);
	m_rows.clear();
}

size_t PlusDocReportInputInterface::RowCount() const
{
	return m_rows.size();
}

const PlusDocReportRow* PlusDocReportInputInterface::RowAt(int index)
{
	return m_rows[index];
}

PlusDocReportRowRange PlusDocReportInputInterface::GetRows()
{
	return PlusDocReportRowRange(m_rows);
}

bool PlusDocReportInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void PlusDocReportInputInterface::Feed(SeasonSession* pSession, const UniqueIds& range)
{
	DeleteRows();
	Consume(pSession,range);
}

void PlusDocReportInputInterface::AddRow(PlusDocReportRow* row)
{
	m_rows.push_back(row);
}


//#################################################################################################

void BuyDocReportInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t docId,range)
	{
		IBuyDoc* pDoc = pSession->GetBuyDoc(docId)->Object();
		uint32_t invoiceId = pDoc->GetInvoiceId();
		boost::optional<InvoiceHent> buyFromHent;

		if (invoiceId != NULL_ID)
		{
			BuyInvoice* buyInvoice = pSession->GetBuyInvoice(invoiceId)->Object();
			buyFromHent = InvoiceHent();
			buyFromHent->CopyFrom(buyInvoice->GetInvoiceHent());
		}

		PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
		while (cowsEnum.hasNext())
		{
			ICowEntry* entry = *cowsEnum;
			ICow* cow = pSession->GetCow(entry->GetCowId())->Object();
			PlusDocReportRow* row = new PlusDocReportRow();
			
			row->cowNo = cow->GetCowNo();
			row->plusDocNo = DocNoText(pDoc).ToString();
			row->buyFromHent = buyFromHent;

			AddRow(row);
			cowsEnum.advance();
		}	

	}
}

//-------------------------------------------------------------------------------------------------
void InDocReportInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t docId, range)
	{
		IInDoc* pDoc = pSession->GetInDoc(docId)->Object();
		
		PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
		while (cowsEnum.hasNext())
		{
			ICowEntry* entry = *cowsEnum;
			ICow* cow = pSession->GetCow(entry->GetCowId())->Object();
			boost::optional<InvoiceHent> buyFromHent;
			uint32_t invoiceId = cow->GetBuyInvoiceId();
			if (invoiceId != NULL_ID)
			{
				BuyInvoice* buyInvoice = pSession->GetBuyInvoice(invoiceId)->Object();
				buyFromHent = InvoiceHent();
				buyFromHent->CopyFrom(buyInvoice->GetInvoiceHent());
			}

			PlusDocReportRow* row = new PlusDocReportRow();

			row->cowNo = cow->GetCowNo();
			row->plusDocNo = DocNoText(pDoc).ToString();
			row->buyFromHent = buyFromHent;

			AddRow(row);
			cowsEnum.advance();
		}

	}
}

