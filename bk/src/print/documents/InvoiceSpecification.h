#ifndef __INVOICESPECIFICATION_H__
#define __INVOICESPECIFICATION_H__

#include "../DocumentInterface.h"
#include "../../bkprint/template/Template.h"
#include <boost\range\any_range.hpp>
#include "../IdsInputInterface.h"
#include "../DocumentPredicate.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "InvoiceDocument.h"
#include <data\IInvoice.h>
#include <boost\optional.hpp>

class InvoiceSpecificationRow
{
public:	
	uint32_t cowId;
	std::wstring herdClassCd;
	std::wstring paidClassCd;	
	Decimal herdWeight;
	Decimal paidWeight;	
	CowNo cowNo;
	Decimal pricePerKg;
	Decimal netValue;
	Decimal vatValue;
	Decimal grossValue;

	boost::optional<HentRecord> buyFromHent;
};

typedef std::vector<InvoiceSpecificationRow*> InvoiceSpecificationRows;

class InvoiceSpecificationRecord
{
public:
	HentRecord buyer;
	HentRecord seller;	
	std::wstring invoiceNo;
	DateTime invoiceDate;	
	
	Decimal totalNet;
	Decimal totalVat;
	Decimal totalGross;
	Decimal totalGrossAfterDeductions;
	
	Decimal vatRate;
	Decimal totalPaidWeight;
	Decimal totalHerdWeight;
	
	InvoiceSpecificationRows rows;	
};

typedef boost::any_range<const InvoiceSpecificationRecord*, boost::forward_traversal_tag, InvoiceSpecificationRecord* const, std::ptrdiff_t> InvoiceSpeciticationRecordRange;

class InvoiceSpecificationInputInterface : public IdsInputInterface
{
public:
	~InvoiceSpecificationInputInterface();
	InvoiceSpeciticationRecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession, const UniqueIds& range);
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range) = 0;
	void AddRecord(InvoiceSpecificationRecord* record);
	void DeleteRecords();
	InvoiceSpecificationRecord* CreateRecord(SeasonSession* session, IInvoice* invoice);
private:
	std::vector<InvoiceSpecificationRecord*> m_records;
};

class BuyInvoiceSpecificationInputInterface : public InvoiceSpecificationInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};

class SellInvoiceSpecificationInputInterface : public InvoiceSpecificationInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};


class InvoiceSpecification : public DocumentInterface
{
	class InvoiceSpecDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(InvoiceSpecDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const InvoiceSpecificationRow* row);			
		private:
			InvoiceSpecDS* m_parent;	
			
		};

		friend class ItemsBandEvaluator;
	public:
		InvoiceSpecDS(const InvoiceSpecificationRecord* record,
						  IPrintProfile* profile,
						  DocumentContext* pContext);
		~InvoiceSpecDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	private:		
		std::wstring FormatHent(const HentRecord* hent);
		DocumentContext* m_pContext;
		const InvoiceSpecificationRecord* m_record;
		IPrintProfile* m_profile;
		ItemsBandEvaluator* m_itemsBandEvaluator;		
		
	};

	class HyperInvoiceSpecDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		HyperInvoiceSpecDS(InvoiceSpecificationInputInterface* inputInterface,IPrintProfile* profile,DocumentContext* pContext);
		~HyperInvoiceSpecDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<InvoiceSpecDS*> m_dataSources;
	};

public:
	InvoiceSpecification(bkprint::Template* templ, InvoiceSpecificationInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);	
	virtual void SetContext(DocumentContext* context);	
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~InvoiceSpecification();

private:
	InvoiceSpecificationInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

template<class INPUTINTERFACE>
class ConcreteInvoiceSpecDocument : public InvoiceSpecification
{
public:
	ConcreteInvoiceSpecDocument(bkprint::Template* templ) : InvoiceSpecification(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef ConcreteInvoiceSpecDocument<BuyInvoiceSpecificationInputInterface> BuyInvoiceSpecification;
typedef ConcreteInvoiceSpecDocument<SellInvoiceSpecificationInputInterface> SellInvoiceSpecification;




#endif
