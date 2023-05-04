#ifndef __PLUSDOCREPORT_H__
#define __PLUSDOCREPORT_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../view/HentView.h"
#include "../../bkprint/template/ExpandableTemplate.h"

class PlusDocReportRow
{
public:
	CowNo cowNo;
	std::wstring plusDocNo;
	boost::optional<InvoiceHent> buyFromHent;
};

typedef boost::any_range<const PlusDocReportRow*,boost::forward_traversal_tag, PlusDocReportRow* const, std::ptrdiff_t> PlusDocReportRowRange;

class PlusDocReportInputInterface : public IdsInputInterface
{
public:
	~PlusDocReportInputInterface();
	PlusDocReportRowRange GetRows();
	size_t RowCount() const;
	const PlusDocReportRow* RowAt(int index);
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRow(PlusDocReportRow* record);
	void DeleteRows();
private:
	std::vector<PlusDocReportRow*> m_rows;
};

class PlusDocReport : public DocumentInterface
{
	class PlusDocReportDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(PlusDocReportDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const PlusDocReportRow *row);
			std::wstring FormatInvoiceHent(const InvoiceHent& hent);
		private:
			PlusDocReportDS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		PlusDocReportDS(PlusDocReport* parent, IPrintProfile* profile);
		~PlusDocReportDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	
		
	private:
		IPrintProfile* m_profile;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		PlusDocReport* m_parent;		
	};

	friend class PlusDocReportDS;
public:
	PlusDocReport(bkprint::Template* templ, PlusDocReportInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~PlusDocReport();
		
protected:
	
protected:
	PlusDocReportInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

class BuyDocReportInputInterface : public PlusDocReportInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class InDocReportInputInterface : public PlusDocReportInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

template<class INPUTINTERFACE>
class PlusDocReportDocument : public PlusDocReport
{
public:
	PlusDocReportDocument(bkprint::Template* templ) : PlusDocReport(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef PlusDocReportDocument<BuyDocReportInputInterface> BuyDocReport;
typedef PlusDocReportDocument<InDocReportInputInterface> InDocReport;

#endif
