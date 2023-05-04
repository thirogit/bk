#ifndef __FOODCHAIN3_H__
#define __FOODCHAIN3_H__

#include "../DocumentInterface.h"
#include "../IdsInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/FixedTemplateDataSource.h"
#include "../../view/HentView.h"
#include "../../bkprint/template/FixedTemplate.h"
#include "DocumentWithInputInterface.h"

class Foodchain3Options
{
public:
	Foodchain3Options(IPrintProfile* profile);	
	const std::wstring& spiecies();
	const std::wstring& slaughter();
	const std::wstring& plateno();
private:
	std::wstring m_spiecies;
	std::wstring m_slaughter;
	std::wstring m_plateno;
};


class Foodchain3Record
{
public:
	Foodchain3Record();
	HentView hent;
	std::vector<CowNo> cowNos;
	std::wstring plateNo;
	DateTime date;
};


typedef boost::any_range<const Foodchain3Record*, boost::forward_traversal_tag, Foodchain3Record* const, std::ptrdiff_t> Foodchain3RecordRange;


class Foodchain3InputInterface : public IdsInputInterface
{
public:
	~Foodchain3InputInterface();
	Foodchain3RecordRange GetRecords();
	size_t RecordCount() const;
	const Foodchain3Record* RecordAt(int index);
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession, const UniqueIds& range);
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range) = 0;
	void AddRecord(Foodchain3Record* record);
	void AddRecord(SeasonSession* pSession, IHentsDoc* pDoc);
	Foodchain3Record* CreateRecord(SeasonSession* pSession, IHentsDoc* pDoc);
	void DeleteRecords();
private:
	std::vector<Foodchain3Record*> m_records;
};


class Foodchain3 : public DocumentInterface
{
	class Foodchain3DS : public bkprint::FixedTemplateDataSource
	{
	public:
		Foodchain3DS(const Foodchain3Record* record, Foodchain3Options* options);
		virtual int GetSheetCount();
		virtual bkprint::Values GetValuesForSheet(int sheet);
	protected:
		
	protected:
		const Foodchain3Record* m_record;
		Foodchain3Options* m_options;
	};

	class HyperFoodchain3DS : public bkprint::HyperFixedTemplateDataSource
	{
	public:
		HyperFoodchain3DS(Foodchain3* parent, Foodchain3Options* options);
		~HyperFoodchain3DS();
		virtual int GetDSCount();
		virtual bkprint::FixedTemplateDataSource* GetDS(int index);
	private:
		std::vector<Foodchain3DS*> m_dataSrcs;
	};

	friend class HyperFoodchain1DS;
public:
	Foodchain3(bkprint::Template* templ, Foodchain3InputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~Foodchain3();
		
protected:
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
	Foodchain3InputInterface* m_inputInterface;
	
};

class Foodchain3BuyDocInputInterface : public Foodchain3InputInterface
{
public:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};

class Foodchain3SellDocInputInterface : public Foodchain3InputInterface
{
public:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};

typedef DocumentWithInputInterface<Foodchain3BuyDocInputInterface, Foodchain3> BuyDocFoodchain3;
typedef DocumentWithInputInterface<Foodchain3SellDocInputInterface, Foodchain3> SellDocFoodchain3;

#endif
