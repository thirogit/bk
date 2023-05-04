#ifndef __HEALTHCERTATTACHMENT_H__
#define __HEALTHCERTATTACHMENT_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../bkprint/template/ExpandableTemplate.h"

class HealthCertAttachmentRow
{
public:
	CowNo cowNo;
	std::wstring passportNo;
};

class HealthCertAttachmentRecord
{
public:
	std::vector<HealthCertAttachmentRow*> rows;
};

typedef boost::any_range<const HealthCertAttachmentRecord*, boost::forward_traversal_tag, HealthCertAttachmentRecord* const, std::ptrdiff_t> HealthCertAttachmentRecordRange;

class HealthCertAttachInputInterface : public IdsInputInterface
{
public:
	~HealthCertAttachInputInterface();
	HealthCertAttachmentRecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(HealthCertAttachmentRecord* record);
	HealthCertAttachmentRecord* CreateRecord(SeasonSession* pSession, IDoc* pDoc);
	void DeleteRecords();
private:
	std::vector<HealthCertAttachmentRecord*> m_records;
};

class HealthCertAttachment : public DocumentInterface
{
	class HealthCertAttachmentDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(HealthCertAttachmentDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const HealthCertAttachmentRow *row);

		private:
			HealthCertAttachmentDS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		HealthCertAttachmentDS(const HealthCertAttachmentRecord* record, IPrintProfile* profile, DocumentContext* pContext);
		~HealthCertAttachmentDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	
	private:
		IPrintProfile* m_profile;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		DocumentContext* m_pContext;
		const HealthCertAttachmentRecord* m_record;
	};

	friend class OwnDocSpecDS;


	class HyperHealthCertAttachmentDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		HyperHealthCertAttachmentDS(HealthCertAttachInputInterface* inputInterface, IPrintProfile* profile, DocumentContext* pContext);
		~HyperHealthCertAttachmentDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<HealthCertAttachmentDS*> m_dataSources;
	};

public:
	HealthCertAttachment(bkprint::Template* templ, HealthCertAttachInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~HealthCertAttachment();
		
protected:
	HealthCertAttachInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

class SellDocHealthCertInputInterface : public HealthCertAttachInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class OutDocHealthCertInputInterface : public HealthCertAttachInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

template<class INPUTINTERFACE>
class HealthCertAttachmentDocument : public HealthCertAttachment
{
public:
	HealthCertAttachmentDocument(bkprint::Template* templ) : HealthCertAttachment(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef HealthCertAttachmentDocument<SellDocHealthCertInputInterface> SellDocHealthCertAttachment;
typedef HealthCertAttachmentDocument<OutDocHealthCertInputInterface> OutDocHealthCertAttachment;

#endif
