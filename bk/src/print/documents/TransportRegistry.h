#ifndef __TRANSPORTREGISTRY_H__
#define __TRANSPORTREGISTRY_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../view/HentView.h"
#include "../../bkprint/template/ExpandableTemplate.h"
#include <boost/optional.hpp>


class TransportRegistryOptions
{
public:
	TransportRegistryOptions(IPrintProfile* profile);
	bool DontFillCarier() const;
	
private:
	bool m_donFillCarrier;
};

class TransportRegistryRow
{
public:
	TransportRegistryRow();

	CowNo cowNo;
	boost::optional<HentView> buyFromHent;	
};

class TransportRegistryRecord
{
public:
	TransportRegistryRecord();

	std::wstring docNumber;
	HentView carrier;
	HentView destination;
	DateTime docDate;
	DateTime transportDate;
	std::wstring plateNo;
	std::wstring extras;
	DateTime loadDate;
	Time loadStartTm;
	Time loadEndTm;
		
	std::vector<TransportRegistryRow*> rows;	
};

typedef boost::any_range<const TransportRegistryRecord*,boost::forward_traversal_tag, TransportRegistryRecord* const, std::ptrdiff_t> TransportRegistryRecordRange;

class TransRegInputInterface : public IdsInputInterface
{
public:
	~TransRegInputInterface();
	TransportRegistryRecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(TransportRegistryRecord* record);
	TransportRegistryRecord* CreateRecord(SeasonSession* pSession,IHentsDoc* pDoc);
	void DeleteRecords();
private:
	std::vector<TransportRegistryRecord*> m_records;
};

class TransportRegistry : public DocumentInterface
{

	class TransportRegDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(TransportRegDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const TransportRegistryRow *row);
			std::wstring FormatFirstOwner(const HentView* hent);
		private:
			TransportRegDS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		TransportRegDS(const TransportRegistryRecord* record, TransportRegistry* parent, const TransportRegistryOptions* options);
		~TransportRegDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	protected:
		std::wstring FormatHent(const HentView* pHent);
	private:
		const TransportRegistryOptions* m_options;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		TransportRegistry* m_parent;
		const TransportRegistryRecord* m_record;
		
	};

	

	class HyperTransportRegDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		HyperTransportRegDS(TransportRegistryRecordRange records,const TransportRegistryOptions* options, TransportRegistry* parent);
		~HyperTransportRegDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<TransportRegDS*> m_dataSources;
	};

public:
	TransportRegistry(bkprint::Template* templ,TransRegInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~TransportRegistry();
	
private:
	TransRegInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
	
};


template<class INPUTINTERFACE>
class TransportRegistryDocument : public TransportRegistry
{
public:
	TransportRegistryDocument(bkprint::Template* templ) : TransportRegistry(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};


class BuyDocTransRegInputInterface : public TransRegInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class SellDocTransRegInputInterface : public TransRegInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

typedef TransportRegistryDocument<BuyDocTransRegInputInterface> BuyDocTransportRegistry;
typedef TransportRegistryDocument<SellDocTransRegInputInterface> SellTransportRegistry;

#endif
