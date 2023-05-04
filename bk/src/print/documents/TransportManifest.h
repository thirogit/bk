#ifndef __TRANSPORTMANIFEST_H__
#define __TRANSPORTMANIFEST_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../bkprint/template/ExpandableTemplate.h"
#include "../../view/HentView.h"


class TransportManifestOptions
{
public:
	TransportManifestOptions(IPrintProfile* profile);

	const std::wstring& spiecies() const;
private:
	std::wstring m_spiecies;
};



class TransportManifestRow
{
public:
	CowNo cowNo;
	std::wstring passNo;
};

class TransportManifestRecord
{
public:
	TransportManifestRecord();
	
	HentView source;
	HentView destination;
		
	std::vector<TransportManifestRow*> rows;	
};

typedef boost::any_range<const TransportManifestRecord*,boost::forward_traversal_tag,  TransportManifestRecord* const, std::ptrdiff_t> TransportManifestRecordRange;

class TransManifInputInterface : public IdsInputInterface
{
public:
	~TransManifInputInterface();
	TransportManifestRecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(TransportManifestRecord* record);	
	void DeleteRecords();
private:
	std::vector<TransportManifestRecord*> m_records;
};

class TransportManifest : public DocumentInterface
{

	class TransManifDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(TransManifDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const TransportManifestRow *row);

		private:
			TransManifDS* m_parent;

		};


		friend class ItemsBandEvaluator;
	public:
		TransManifDS(const TransportManifestRecord* record, TransportManifest* parent, TransportManifestOptions* options);
		~TransManifDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	protected:
		std::wstring FormatAddress(const HentView& hent);
	private:
		TransportManifestOptions* m_options;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		TransportManifest* m_parent;
		const TransportManifestRecord* m_record;
	};

	

	class HyperTransManifDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		HyperTransManifDS(TransportManifestRecordRange records, TransportManifestOptions* options, TransportManifest* parent);
		~HyperTransManifDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<TransManifDS*> m_dataSources;
	};




public:
	TransportManifest(bkprint::Template* templ, TransManifInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~TransportManifest();
protected:
	
private:
	TransManifInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
	
};


template<class INPUTINTERFACE>
class TransportManifestDocument : public TransportManifest
{
public:
	TransportManifestDocument(bkprint::Template* templ) : TransportManifest(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

class SellDocTransManifInputInterface : public TransManifInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
private:
	TransportManifestRecord* CreateRecord(SeasonSession* pSession,ISellDoc* pDoc);
};

class MoveDocTransManifInputInterface : public TransManifInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
private:
	TransportManifestRecord* CreateRecord(SeasonSession* pSession,IMoveDoc* pDoc);
};

typedef TransportManifestDocument<MoveDocTransManifInputInterface> MoveDocTransportManifest;
typedef TransportManifestDocument<SellDocTransManifInputInterface> SellTransportManifest;

#endif
