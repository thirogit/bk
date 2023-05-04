#ifndef __NORMALIZEDFOODCHAIN_H__
#define __NORMALIZEDFOODCHAIN_H__

#include "../DocumentInterface.h"
#include "../../bkprint/template/Template.h"
#include <boost\range\any_range.hpp>
#include "../IdsInputInterface.h"
#include "../DocumentPredicate.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include <data\IInvoice.h>
#include "InvoiceDocument.h"


class NormalizedFoodchainOptions
{
public:
	NormalizedFoodchainOptions(IPrintProfile* profile);
	const std::wstring& speicies();
private:
	std::wstring m_spiecies;
};

class NormalizedFoodchainRow
{
public:		
	CowNo cowNo;
	std::wstring passNo;	
};

typedef std::vector<NormalizedFoodchainRow*> NormalizedFoodchainRows;

class NormalizedFoodchainRecord
{
public:
	HentRecord buyFromHent;	
	NormalizedFoodchainRows rows;
};

typedef boost::any_range<const NormalizedFoodchainRecord*, boost::forward_traversal_tag, NormalizedFoodchainRecord* const, std::ptrdiff_t> NormalizedFoodchainRecordRange;

class NormFoodchainInputInterface : public IdsInputInterface
{
public:
	~NormFoodchainInputInterface();
	NormalizedFoodchainRecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession, const UniqueIds& range);
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range) = 0;
	void AddRecord(NormalizedFoodchainRecord* record);
	void DeleteRecords();
	NormalizedFoodchainRecord* CreateRecord(SeasonSession* session, IInvoice* invoice);
private:
	std::vector<NormalizedFoodchainRecord*> m_records;
};

class BuyNormFoodchainInputInterface : public NormFoodchainInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};


class NormalizedFoodchain : public DocumentInterface
{
	class NormFoodchainDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(NormFoodchainDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const NormalizedFoodchainRow* row);
		private:
			NormFoodchainDS* m_parent;
			
		};

		friend class ItemsBandEvaluator;
	public:
		NormFoodchainDS(const NormalizedFoodchainRecord* record,
						NormalizedFoodchainOptions* options,
					    DocumentContext* pContext);
		~NormFoodchainDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	private:		
		std::wstring FormatHentAddress(const HentRecord* hent);
		DocumentContext* m_pContext;
		const NormalizedFoodchainRecord* m_record;
		NormalizedFoodchainOptions* m_options;
		ItemsBandEvaluator* m_itemsBandEvaluator;		
		
	};

	class HyperNormFoodchainDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		HyperNormFoodchainDS(NormFoodchainInputInterface* inputInterface, NormalizedFoodchainOptions* options, DocumentContext* pContext);
		~HyperNormFoodchainDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<NormFoodchainDS*> m_dataSources;
	};

public:
	NormalizedFoodchain(bkprint::Template* templ, NormFoodchainInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);	
	virtual void SetContext(DocumentContext* context);	
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~NormalizedFoodchain();

private:
	NormFoodchainInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

template<class INPUTINTERFACE>
class ConcreteNormalizedFoodchainDocument : public NormalizedFoodchain
{
public:
	ConcreteNormalizedFoodchainDocument(bkprint::Template* templ) : NormalizedFoodchain(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef ConcreteNormalizedFoodchainDocument<BuyNormFoodchainInputInterface> BuyNormalizedFoodchain;





#endif
