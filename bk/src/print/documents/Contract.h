#ifndef __CONTRACT_H__
#define __CONTRACT_H__

#include "../DocumentInterface.h"
#include "../../bkprint/template/Template.h"
#include <boost\range\any_range.hpp>
#include "../IdsInputInterface.h"
#include "../DocumentPredicate.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "InvoiceDocument.h"
#include <data\IInvoice.h>
#include <boost\optional.hpp>

class ContractRow
{
public:	
	CowNo cowNo;
	Decimal pricePerKg;
	Decimal grossValue;
	Decimal paidWeight;
};

typedef std::vector<ContractRow*> ContractRows;

class ContractRecord
{
public:
	HentRecord buyer;
	HentRecord seller;	
	std::wstring invoiceNo;
	DateTime transactionDate;
	NullInt payDueDays;
	ContractRows rows;	
};

typedef boost::any_range<const ContractRecord*, boost::forward_traversal_tag, ContractRecord* const, std::ptrdiff_t> ContractRecordRange;

class ContractInputInterface : public IdsInputInterface
{
public:
	~ContractInputInterface();
	ContractRecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession, const UniqueIds& range);
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range) = 0;
	void AddRecord(ContractRecord* record);
	void DeleteRecords();
	ContractRecord* CreateRecord(SeasonSession* session, IInvoice* invoice);
private:
	std::vector<ContractRecord*> m_records;
};

class BuyInvoiceContractInputInterface : public ContractInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};


class ContractDocument : public DocumentInterface
{
	class ContractDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(ContractDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const ContractRow* row);
		private:
			ContractDS* m_parent;			
		};

		friend class ItemsBandEvaluator;
	public:
		ContractDS(const ContractRecord* record,
						  IPrintProfile* profile,
						  DocumentContext* pContext);
		~ContractDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	private:		
		std::wstring FormatHent(const HentRecord* hent);
		DocumentContext* m_pContext;
		const ContractRecord* m_record;
		IPrintProfile* m_profile;
		ItemsBandEvaluator* m_itemsBandEvaluator;		
		
	};

	class ContractHyperDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		ContractHyperDS(ContractInputInterface* inputInterface,IPrintProfile* profile,DocumentContext* pContext);
		~ContractHyperDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<ContractDS*> m_dataSources;
	};

public:
	ContractDocument(bkprint::Template* templ);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);	
	virtual void SetContext(DocumentContext* context);	
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~ContractDocument();

private:
	ContractInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};



#endif
