#ifndef __OWNDOCSPECIFICATION_H__
#define __OWNDOCSPECIFICATION_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../bkprint/template/ExpandableTemplate.h"

class OwnDocSpecificationRow
{
public:
	CowNo cowNo;
	std::wstring classCd;
	CowSex sex;
	Decimal weight;
	NullInt group;	
};

class OwnDocSpecificationRecord
{
public:
	DateTime docDate;
	DateTime loadDate;
	std::wstring plateNo;
	std::wstring extras;
	std::wstring docNumber;
	Decimal totalWeight;
		
	std::vector<OwnDocSpecificationRow*> rows;	
};

typedef boost::any_range<const OwnDocSpecificationRecord*,boost::forward_traversal_tag, OwnDocSpecificationRecord* const, std::ptrdiff_t> OwnDocSpecRecordRange;

class OwnDocSpecInputInterface : public IdsInputInterface
{
public:
	~OwnDocSpecInputInterface();
	OwnDocSpecRecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(OwnDocSpecificationRecord* record);
	OwnDocSpecificationRecord* CreateRecord(SeasonSession* pSession,IOwnDoc* pDoc);
	void DeleteRecords();
private:
	std::vector<OwnDocSpecificationRecord*> m_records;
};

class OwnDocSpecification : public DocumentInterface
{
	class OwnDocSpecDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(OwnDocSpecDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const OwnDocSpecificationRow *row);

		private:
			OwnDocSpecDS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		OwnDocSpecDS(const OwnDocSpecificationRecord* record, OwnDocSpecification* parent, IPrintProfile* profile);
		~OwnDocSpecDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	
	private:
		IPrintProfile* m_profile;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		OwnDocSpecification* m_parent;
		const OwnDocSpecificationRecord* m_record;
	};

	class HyperOwnDocSpecDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		HyperOwnDocSpecDS(OwnDocSpecRecordRange records, OwnDocSpecification* parent);
		~HyperOwnDocSpecDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<OwnDocSpecDS*> m_dataSources;
	};


	friend class OwnDocSpecDS;


public:
	OwnDocSpecification(bkprint::Template* templ,OwnDocSpecInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~OwnDocSpecification();
		
protected:
protected:
	OwnDocSpecInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

class InDocSpecInputInterface : public OwnDocSpecInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class OutDocSpecInputInterface : public OwnDocSpecInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

template<class INPUTINTERFACE>
class OwnDocSpecificationDocument : public OwnDocSpecification
{
public:
	OwnDocSpecificationDocument(bkprint::Template* templ) : OwnDocSpecification(templ,&m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef OwnDocSpecificationDocument<InDocSpecInputInterface> InDocSpecification;
typedef OwnDocSpecificationDocument<OutDocSpecInputInterface> OutDocSpecification;

#endif
