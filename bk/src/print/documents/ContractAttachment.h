#ifndef __CONTRACTATTACHMENT_H__
#define __CONTRACTATTACHMENT_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include "../../bkprint/template/ExpandableTemplate.h"
#include "../../view/HentView.h"



class ContractAttachmentOptions
{
public:
	ContractAttachmentOptions(IPrintProfile* profile);
	const std::wstring& GetFarmer() const;
	const DateTime& GetContractDate() const;
	const DateTime& GetDeliveryDate() const;
private:
	DateTime m_contractDt;
	DateTime m_deliveryDt;
	std::wstring m_farmer;
};

class ContractAttachmentRow
{
public:
	CowNo cowNo;
	std::wstring passportNo;
	std::wstring stockCd;
	CowSex sex;
	Decimal weight;	
};

class ContractAttachmentRecord
{
public:	
	DateTime docDate;
	DateTime loadDate;
	HentView* company;
	std::wstring farmer;	
	Decimal totalWeight;
	std::vector<ContractAttachmentRow*> rows;	
};

typedef boost::any_range<const ContractAttachmentRecord*,boost::forward_traversal_tag, ContractAttachmentRecord* const, std::ptrdiff_t> ContractAttachmentRecordRange;

class ContractAttachmentInputInterface : public IdsInputInterface
{
public:
	~ContractAttachmentInputInterface();
	ContractAttachmentRecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(ContractAttachmentRecord* record);
	ContractAttachmentRecord* CreateRecord(SeasonSession* pSession,IOwnDoc* pDoc);
	void DeleteRecords();
private:
	std::vector<ContractAttachmentRecord*> m_records;
};

class ContractAttachment : public DocumentInterface
{
	class ContractAttachmentDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(ContractAttachmentDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const ContractAttachmentRow *row);

		private:
			ContractAttachmentDS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		ContractAttachmentDS(const ContractAttachmentRecord* record, ContractAttachment* parent, ContractAttachmentOptions* options);
		~ContractAttachmentDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	protected:
		std::wstring FormatHent(const HentView* pHent);
	private:
		ContractAttachmentOptions* m_options;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		ContractAttachment* m_parent;
		const ContractAttachmentRecord* m_record;
	};

	class HyperContractAttachmentDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		HyperContractAttachmentDS(ContractAttachmentRecordRange records, ContractAttachment* parent, ContractAttachmentOptions* options);
		~HyperContractAttachmentDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<ContractAttachmentDS*> m_dataSources;
	};


	friend class ContractAttachmentDS;


public:
	ContractAttachment(bkprint::Template* templ,ContractAttachmentInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~ContractAttachment();
		
protected:
protected:
	ContractAttachmentInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

// class InDocSpecInputInterface : public OwnDocSpecInputInterface
// {
// public:
	// virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
// };

class OutDocContractAttachmentInputInterface : public ContractAttachmentInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

template<class INPUTINTERFACE>
class ContractAttachmentDocument : public ContractAttachment
{
public:
	ContractAttachmentDocument(bkprint::Template* templ) : ContractAttachment(templ,&m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef ContractAttachmentDocument<OutDocContractAttachmentInputInterface> OutDocContractAttachment;

#endif
