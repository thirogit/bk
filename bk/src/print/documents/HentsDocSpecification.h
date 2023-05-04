#ifndef __HENTSDOCSPECIFICATION_H__
#define __HENTSDOCSPECIFICATION_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../view/HentView.h"
#include "../../bkprint/template/ExpandableTemplate.h"

class HentsDocSpecificationRow
{
public:
	CowNo cowNo;
	std::wstring classCd;
	CowSex sex;
	Decimal weight;
	NullInt group;	
};

class HentsDocSpecificationRecord
{
public:
	HentsDocSpecificationRecord();

	std::wstring docNumber;
	DateTime docDate;
	DateTime loadDate;
	std::wstring plateNo;
	std::wstring extras;
	HentView fromHent;
	HentView toHent;
	Decimal totalWeight;

	std::vector<HentsDocSpecificationRow*> rows;	
};

typedef boost::any_range<const HentsDocSpecificationRecord*,boost::forward_traversal_tag, HentsDocSpecificationRecord* const, std::ptrdiff_t> HentsDocSpecRecordRange;

class HentsDocSpecInputInterface : public IdsInputInterface
{
public:
	~HentsDocSpecInputInterface();
	HentsDocSpecRecordRange GetRecords();
	size_t RecordCount() const;
	const HentsDocSpecificationRecord* RecordAt(int index);
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(HentsDocSpecificationRecord* record);
	HentsDocSpecificationRecord* CreateRecord(SeasonSession* pSession,IHentsDoc* pDoc);
	

	void DeleteRecords();
private:
	std::vector<HentsDocSpecificationRecord*> m_records;
};

class HentsDocSpecification : public DocumentInterface
{


	


	class HentsDocSpecDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(HentsDocSpecDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const HentsDocSpecificationRow *row);
			
		private:
			HentsDocSpecDS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		HentsDocSpecDS(const HentsDocSpecificationRecord* record, HentsDocSpecification* parent, IPrintProfile* profile);
		~HentsDocSpecDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	protected:
		std::wstring FormatHent(const HentView* pHent);
	private:
		IPrintProfile* m_profile;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		HentsDocSpecification* m_parent;
		const HentsDocSpecificationRecord* m_record;
	};

	class HyperHentsDocSpecDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		HyperHentsDocSpecDS(HentsDocSpecRecordRange records, HentsDocSpecification* parent);
		~HyperHentsDocSpecDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<HentsDocSpecDS*> m_dataSources;
	};

	friend class HentsDocSpecDS;
public:
	HentsDocSpecification(bkprint::Template* templ,HentsDocSpecInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~HentsDocSpecification();
		
protected:
	
protected:
	HentsDocSpecInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

class BuyDocSpecInputInterface : public HentsDocSpecInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class SellDocSpecInputInterface : public HentsDocSpecInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

template<class INPUTINTERFACE>
class HentsDocSpecificationDocument : public HentsDocSpecification
{
public:
	HentsDocSpecificationDocument(bkprint::Template* templ) : HentsDocSpecification(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef HentsDocSpecificationDocument<BuyDocSpecInputInterface> BuyDocSpecification;
typedef HentsDocSpecificationDocument<SellDocSpecInputInterface> SellDocSpecification;

#endif
