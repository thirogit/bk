#ifndef __INVOICESSUMMARY_H__
#define __INVOICESSUMMARY_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../view/InvoiceView.h"
#include "../../bkprint/template/ExpandableTemplate.h"

typedef boost::any_range<const InvoiceView*,boost::forward_traversal_tag, InvoiceView* const, std::ptrdiff_t> InvoiceViewRange;

class InvoicesSummaryInputInterface : public IdsInputInterface
{
public:
	~InvoicesSummaryInputInterface();
	InvoiceViewRange GetRows();
	size_t RowCount() const;
	const InvoiceView* RowAt(int index);
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);


	const Decimal& GetTotalTax() const;
	const Decimal& GetTotalNet() const;
	const Decimal& GetTotalGross() const;
	const Decimal& GetTotalWeight() const;
	const Decimal& GetTotalGrossAfterDeductions() const;
	int GetTotalCowAmount() const;
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRow(InvoiceView* record);
	void DeleteRows();

	std::vector<InvoiceView*> m_rows;
	Decimal m_totalTax;
	Decimal m_totalNet;
	Decimal m_totalGross;
	Decimal m_totalWeight;
	Decimal m_totalGrossAfterDeductions;
	int m_totalCowAmount;

};

class InvoicesSummary : public DocumentInterface
{
	class InvoicesSummaryDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(InvoicesSummaryDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const InvoiceView *row);
		private:
			InvoicesSummaryDS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		InvoicesSummaryDS(InvoicesSummary* parent, IPrintProfile* profile);
		~InvoicesSummaryDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	
		
	private:
		IPrintProfile* m_profile;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		InvoicesSummary* m_parent;		
	};

	friend class InvoicesSummaryDS;
public:
	InvoicesSummary(bkprint::Template* templ, InvoicesSummaryInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~InvoicesSummary();
		
protected:
	
protected:
	InvoicesSummaryInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

class BuyInvoicesSummaryInputInterface : public InvoicesSummaryInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class SellInvoicesSummaryInputInterface : public InvoicesSummaryInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

template<class INPUTINTERFACE>
class InvoicesSummaryDocument : public InvoicesSummary
{
public:
	InvoicesSummaryDocument(bkprint::Template* templ) : InvoicesSummary(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef InvoicesSummaryDocument<BuyInvoicesSummaryInputInterface> BuyInvoicesSummaryDocument;
typedef InvoicesSummaryDocument<SellInvoicesSummaryInputInterface> SellInvoicesSummaryDocument;

#endif
