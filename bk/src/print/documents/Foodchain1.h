#ifndef __FOODCHAIN1_H__
#define __FOODCHAIN1_H__

#include "../DocumentInterface.h"
#include "../KeysInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/FixedTemplateDataSource.h"
#include <data\impl\InvoiceHent.h>
#include "../../bkprint/template/FixedTemplate.h"


class Foodchain1Options
{
public:
	Foodchain1Options(IPrintProfile* profile);

	
private:
	
};


class Foodchain1Record
{
public:
	boost::optional<InvoiceHent> buyFromHent;
	CowNo cowNo;
};

typedef boost::any_range<const Foodchain1Record*, boost::forward_traversal_tag, Foodchain1Record* const, std::ptrdiff_t> Foodchain1RecordRange;


class Foodchain1 : public DocumentInterface,private KeysInputInterface
{
	class Foodchain1DS : public bkprint::FixedTemplateDataSource
	{
	public:
		Foodchain1DS(const Foodchain1Record* record, Foodchain1Options* options);
		virtual int GetSheetCount();
		virtual bkprint::Values GetValuesForSheet(int sheet);
	protected:
		
	protected:
		const Foodchain1Record* m_record;
		Foodchain1Options* m_options;
	};

	class HyperFoodchain1DS : public bkprint::HyperFixedTemplateDataSource
	{
	public:
		HyperFoodchain1DS(Foodchain1* parent, Foodchain1Options* options);
		~HyperFoodchain1DS();
		virtual int GetDSCount();
		virtual bkprint::FixedTemplateDataSource* GetDS(int index);
	private:
		std::vector<Foodchain1DS*> m_dataSrcs;
	};

	friend class HyperFoodchain1DS;
public:
	Foodchain1(bkprint::Template* templ);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void Feed(SeasonSession* pSession,const UniquePresenceKeys& keys);
	virtual void SetContext(DocumentContext* context);
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~Foodchain1();
		
protected:
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
	std::vector<Foodchain1Record*> m_records;
};

#endif
