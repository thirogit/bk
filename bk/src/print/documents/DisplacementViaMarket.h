#ifndef __DISPLACEMENTVIAMARKET_H__
#define __DISPLACEMENTVIAMARKET_H__

#include "../DocumentInterface.h"
#include "../../bkprint/template/FixedTemplate.h"
#include "../../bkprint/renderer/FixedTemplateDataSource.h"
#include <set>
#include <boost\range\any_range.hpp>
#include "../../runtime/FarmStructure.h"
#include "../../runtime/UniqueIds.h"
#include "../KeysInputInterface.h"
#include <boost/range/any_range.hpp>


struct DisplacementViaMarketRecordRow
{
	CowNo cowNo;
	HentNo hentNoFrom;
	HentNo hentNoTo;
};

class DisplacementViaMarketOptions
{
public:
	DisplacementViaMarketOptions(IPrintProfile* profile);
	HentNo GetMarketNo() const;
	HentNo GetSrcHentNo() const;
	HentNo GetDstHentNo() const;

private:
	HentNo m_MarketNo;	
	HentNo m_SrcHentNo;
	HentNo m_DstHentNo;
};

typedef boost::any_range<const DisplacementViaMarketRecordRow*, boost::forward_traversal_tag, DisplacementViaMarketRecordRow* const, std::ptrdiff_t> DisplacementViaMarketRowsRange;

class DisplacementViaMarketRecord
{
public:
	DisplacementViaMarketRecord();
	~DisplacementViaMarketRecord();
	void SetEventDate(const DateTime& eventDt);
	const DateTime& GetEventDate() const;	
	
	
	void AddRow(const CowNo& cowNo,const HentNo& hentNoFrom,const HentNo& hentNoTo);	

	size_t GetRowCount() const;
	DisplacementViaMarketRowsRange GetRows() const;

private:
	std::set<DisplacementViaMarketRecordRow*> m_rows;
	DateTime m_EventDate;	

};

typedef boost::any_range<const DisplacementViaMarketRecord*,boost::forward_traversal_tag, DisplacementViaMarketRecord* const, std::ptrdiff_t> DisplacementViaMarketRecordsRange;

class DisplacementViaMarketDocument : public DocumentInterface, private KeysInputInterface
{
	class DisplacementViaMarketNotificationDS : public bkprint::FixedTemplateDataSource
	{
	public:
		DisplacementViaMarketNotificationDS(const DisplacementViaMarketRecord* record,const DisplacementViaMarketOptions* options);
		virtual int GetSheetCount();
		virtual bkprint::Values GetValuesForSheet(int sheet);
	protected:
		uint32_t GetEvents();
		bkprint::Values GetValuesForCharacters(const std::wstring& s, int length, const std::wstring& valueNameFmt);
	protected:
		const DisplacementViaMarketRecord* m_record;
		const DisplacementViaMarketOptions* m_options;
		
	};


	class HyperDisplacementViaMarketNotificationDS : public bkprint::HyperFixedTemplateDataSource
	{
	public:
		HyperDisplacementViaMarketNotificationDS(DisplacementViaMarketDocument* parent,const DisplacementViaMarketOptions* options);
		~HyperDisplacementViaMarketNotificationDS();
		virtual int GetDSCount();
		virtual bkprint::FixedTemplateDataSource* GetDS(int index);
	private:		
		std::vector<DisplacementViaMarketNotificationDS*> m_dataSrcs;
	};

	friend class HyperDisplacementViaMarketNotificationDS;
protected:
	
	virtual bool QueryInputInterface(const type_info& interfase_type, void** interfase);
	virtual void Feed(SeasonSession* pSession, const UniquePresenceKeys& keys);
	void AddRecord(DisplacementViaMarketRecord* record);
	void DeleteRecords();


	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	
public:	
	DisplacementViaMarketDocument(bkprint::Template* templ);
	virtual ~DisplacementViaMarketDocument();
	virtual void SetContext(DocumentContext* context);
protected:	
	std::vector<DisplacementViaMarketRecord*> m_records;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};




#endif
