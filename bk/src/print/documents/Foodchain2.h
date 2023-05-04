#ifndef __FOODCHAIN2_H__
#define __FOODCHAIN2_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/FixedTemplateDataSource.h"
#include "../../view/HentView.h"
#include "../../bkprint/template/FixedTemplate.h"
#include "DocumentWithInputInterface.h"

class Foodchain2Options
{
public:
	Foodchain2Options(IPrintProfile* profile);	
	const std::wstring& spiecies();
	const std::wstring& slaughter();
	const std::wstring& plateno();
private:
	std::wstring m_spiecies;
	std::wstring m_slaughter;
	std::wstring m_plateno;
};


class Foodchain2Record
{
public:
	Foodchain2Record();
	HentView hent;
	std::vector<CowNo> cowNos;
	std::wstring plateNo;
	DateTime date;
};


typedef boost::any_range<const Foodchain2Record*, boost::forward_traversal_tag, Foodchain2Record* const, std::ptrdiff_t> Foodchain2RecordRange;


class Foodchain2InputInterface : public IdsInputInterface
{
public:
	~Foodchain2InputInterface();
	Foodchain2RecordRange GetRecords();
	size_t RecordCount() const;
	const Foodchain2Record* RecordAt(int index);
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession, const UniqueIds& range);
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range) = 0;
	void AddRecord(Foodchain2Record* record);
	void AddRecord(SeasonSession* pSession, IHentsDoc* pDoc);
	Foodchain2Record* CreateRecord(SeasonSession* pSession, IHentsDoc* pDoc);
	void DeleteRecords();
private:
	std::vector<Foodchain2Record*> m_records;
};


class Foodchain2 : public DocumentInterface
{
	class Foodchain2DS : public bkprint::FixedTemplateDataSource
	{
	public:
		Foodchain2DS(const Foodchain2Record* record, Foodchain2Options* options);
		virtual int GetSheetCount();
		virtual bkprint::Values GetValuesForSheet(int sheet);
	protected:
		
	protected:
		const Foodchain2Record* m_record;
		Foodchain2Options* m_options;
	};

	class HyperFoodchain2DS : public bkprint::HyperFixedTemplateDataSource
	{
	public:
		HyperFoodchain2DS(Foodchain2* parent, Foodchain2Options* options);
		~HyperFoodchain2DS();
		virtual int GetDSCount();
		virtual bkprint::FixedTemplateDataSource* GetDS(int index);
	private:
		std::vector<Foodchain2DS*> m_dataSrcs;
	};

	friend class HyperFoodchain1DS;
public:
	Foodchain2(bkprint::Template* templ, Foodchain2InputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~Foodchain2();
		
protected:
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
	Foodchain2InputInterface* m_inputInterface;
	
};

class Foodchain2BuyDocInputInterface : public Foodchain2InputInterface
{
public:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};

class Foodchain2SellDocInputInterface : public Foodchain2InputInterface
{
public:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};

typedef DocumentWithInputInterface<Foodchain2BuyDocInputInterface, Foodchain2> BuyDocFoodchain2;
typedef DocumentWithInputInterface<Foodchain2SellDocInputInterface, Foodchain2> SellDocFoodchain2;

#endif
