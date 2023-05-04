#ifndef __DISINFECTION_H__
#define __DISINFECTION_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/FixedTemplateDataSource.h"
#include "../../view/HentView.h"
#include "../../bkprint/template/FixedTemplate.h"


class DisinfectionOptions
{
public:
	DisinfectionOptions(IPrintProfile* profile);

	const std::wstring& chemicalAgent() const;
	bool dontPutPlateNo() const;
private:
	std::wstring m_chemicalAgent;
	bool m_dontPutPlateNo;
};

class DisinfectionRecord
{
public:
	DisinfectionRecord();

	std::wstring docNumber;
	DateTime docDate;
	DateTime loadDate;
	std::wstring plateNo;
	std::wstring extras;
	HentView fromHent;
	HentView toHent;
	size_t animalCount;
};

typedef boost::any_range<const DisinfectionRecord*, boost::forward_traversal_tag, DisinfectionRecord* const, std::ptrdiff_t> DisinfectionRecordRange;

class DisinfectionDocInputInterface : public IdsInputInterface
{
public:
	~DisinfectionDocInputInterface();
	DisinfectionRecordRange GetRecords();
	size_t RecordCount() const;
	const DisinfectionRecord* RecordAt(int index);
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession,const UniqueIds& range);
protected:	
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range) = 0;
	void AddRecord(DisinfectionRecord* record);
	DisinfectionRecord* CreateRecord(SeasonSession* pSession, IHentsDoc* pDoc);
	

	void DeleteRecords();
private:
	std::vector<DisinfectionRecord*> m_records;
};

class Disinfection : public DocumentInterface
{
	class DisinfectionDS : public bkprint::FixedTemplateDataSource
	{
	public:
		DisinfectionDS(const DisinfectionRecord* record, DisinfectionOptions* options);
		virtual int GetSheetCount();
		virtual bkprint::Values GetValuesForSheet(int sheet);
	protected:
		
	protected:
		const DisinfectionRecord* m_record;
		DisinfectionOptions* m_options;
	};

	class HyperDisinfectionDS : public bkprint::HyperFixedTemplateDataSource
	{
	public:
		HyperDisinfectionDS(Disinfection* parent, DisinfectionOptions* options);
		~HyperDisinfectionDS();
		virtual int GetDSCount();
		virtual bkprint::FixedTemplateDataSource* GetDS(int index);
	private:
		std::vector<DisinfectionDS*> m_dataSrcs;
	};

	friend class HyperDisinfectionDS;
public:
	Disinfection(bkprint::Template* templ, DisinfectionDocInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~Disinfection();
		
protected:
	
protected:
	DisinfectionDocInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};

class BuyDocDisinfectionInputInterface : public DisinfectionDocInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

class SellDocDisinfectionInputInterface : public DisinfectionDocInputInterface
{
public:
	virtual void Consume(SeasonSession* pSession,const UniqueIds& range);
};

template<class INPUTINTERFACE>
class DisinfectionDocument : public Disinfection
{
public:
	DisinfectionDocument(bkprint::Template* templ) : Disinfection(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};

typedef DisinfectionDocument<BuyDocDisinfectionInputInterface> BuyDocDisinfectionDocument;
typedef DisinfectionDocument<SellDocDisinfectionInputInterface> SellDocDisinfectionDocument;

#endif
