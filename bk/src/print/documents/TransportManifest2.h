#ifndef __TRANSPORTMANIFEST2_H__
#define __TRANSPORTMANIFEST2_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../bkprint/template/ExpandableTemplate.h"
#include "../../view/HentView.h"
#include <data\impl\Herd.h>
#include "DocumentWithInputInterface.h"

class TransportManifest2Options
{
public:
	TransportManifest2Options(IPrintProfile* profile);

	const std::wstring& spiecies() const;
private:
	std::wstring m_spiecies;
};


class TransportManifest2Row
{
public:
	CowNo cowNo;
	std::wstring className;
	boost::optional<InvoiceHent> buyFromHent;
	Decimal weight;
};

class TransportManifest2Record
{
public:
	TransportManifest2Record();
	
	HentView source;
	HentView destination;
	Herd srcHerd;
	std::wstring plateNo;
	DateTime loadDate;
	Time loadTime;

	std::vector<TransportManifest2Row*> rows;	
};

typedef boost::any_range<const TransportManifest2Record*, boost::forward_traversal_tag,  TransportManifest2Record* const, std::ptrdiff_t> TransportManifest2RecordRange;

class TransManif2InputInterface : public IdsInputInterface
{
public:
	~TransManif2InputInterface();
	TransportManifest2RecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(TransportManifest2Record* record);	
	void DeleteRecords();
private:
	std::vector<TransportManifest2Record*> m_records;
};

class TransportManifest2 : public DocumentInterface
{

	class TransManif2DS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(TransManif2DS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const TransportManifest2Row *row);

		private:
			TransManif2DS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		TransManif2DS(const TransportManifest2Record* record, TransportManifest2* parent, TransportManifest2Options* options);
		~TransManif2DS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	protected:
		std::wstring FormatAddress(const HentView& hent);
	private:
		TransportManifest2Options* m_options;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		TransportManifest2* m_parent;
		const TransportManifest2Record* m_record;
	};

	friend class OwnDocSpecDS;



public:
	TransportManifest2(bkprint::Template* templ, TransManif2InputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~TransportManifest2();
protected:
	
private:
	TransManif2InputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
	
};


class BuyDocTransManif2InputInterface : public TransManif2InputInterface
{
public:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
private:
	TransportManifest2Record* CreateRecord(SeasonSession* pSession, IBuyDoc* pDoc);
};

class SellDocTransManif2InputInterface : public TransManif2InputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
private:
	TransportManifest2Record* CreateRecord(SeasonSession* pSession,ISellDoc* pDoc);
};

class MoveDocTransManif2InputInterface : public TransManif2InputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
private:
	TransportManifest2Record* CreateRecord(SeasonSession* pSession,IMoveDoc* pDoc);
};

typedef DocumentWithInputInterface<MoveDocTransManif2InputInterface, TransportManifest2> MoveDocTransportManifest2;
typedef DocumentWithInputInterface<SellDocTransManif2InputInterface, TransportManifest2> SellTransportManifest2;
typedef DocumentWithInputInterface<BuyDocTransManif2InputInterface, TransportManifest2> BuyTransportManifest2;

#endif
