#ifndef __WEIGHTSPEC_H__
#define __WEIGHTSPEC_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../view/HentView.h"
#include "../../bkprint/template/ExpandableTemplate.h"

class WeightSpecOptions
{
public:
	WeightSpecOptions(IPrintProfile* profile);
	bool GetDontFillDeliver();
private:
	bool m_DontFillDeliver;
};

class WeightSpecRow
{
public:
	CowNo cowNo;
	std::wstring classCd;
	CowSex sex;
	Decimal weight;
};

class WeightSpecRecord
{
public:
	WeightSpecRecord();

	std::wstring docNumber;
	DateTime docDate;
	DateTime loadDate;
	Time loadTm;
	Time unloadTm;
	std::wstring plateNo;
	std::wstring extras;
	HentView fromHent;
	HentView toHent;
	
	std::vector<WeightSpecRow*> rows;	
};

typedef boost::any_range<const WeightSpecRecord*,boost::forward_traversal_tag,  WeightSpecRecord* const, std::ptrdiff_t> WeightSpecRecordRange;

class WeightSpecInputInterface : public IdsInputInterface
{
public:
	~WeightSpecInputInterface();
	WeightSpecRecordRange GetRecords();
	size_t RecordCount() const;
	const WeightSpecRecord* RecordAt(int index);
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(WeightSpecRecord* record);
	WeightSpecRecord* CreateRecord(SeasonSession* pSession, IDoc* pDoc);
	void DeleteRecords();
private:
	std::vector<WeightSpecRecord*> m_records;
};

class WeightSpecification : public DocumentInterface
{
	class WeightSpecDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(WeightSpecDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const WeightSpecRow *row);
			
		private:
			WeightSpecDS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		WeightSpecDS(const WeightSpecRecord* record, WeightSpecification* parent, IPrintProfile* profile);
		~WeightSpecDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	protected:
		std::wstring FormatHent(const HentView* pHent);
	private:
		WeightSpecOptions m_options;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		WeightSpecification* m_parent;
		const WeightSpecRecord* m_record;
	};

	friend class WeightSpecDS;
public:
	WeightSpecification(bkprint::Template* templ,WeightSpecInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~WeightSpecification();
		
protected:
	
protected:
	WeightSpecInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

class BuyDocWeightSpecInputInterface : public WeightSpecInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class SellDocWeightSpecInputInterface : public WeightSpecInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

template<class INPUTINTERFACE>
class WeightSpecificationDocument : public WeightSpecification
{
public:
	WeightSpecificationDocument(bkprint::Template* templ) : WeightSpecification(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef WeightSpecificationDocument<BuyDocWeightSpecInputInterface> BuyDocWeightSpecification;
typedef WeightSpecificationDocument<SellDocWeightSpecInputInterface> SellDocWeightSpecification;

#endif
