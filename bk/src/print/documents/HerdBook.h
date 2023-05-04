#ifndef __HERDBOOK_H__
#define __HERDBOOK_H__

#include <vector>
#include "../../bkprint/template/ExpandableTemplate.h"
#include "../../view/CowPresenceView.h"
#include "../DocumentInterface.h"
#include "../KeysInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"


class HerdBookOptions
{
public:
	HerdBookOptions(IPrintProfile* profile);

	int recordIndex() const;
	int pageIndex() const;
	bool useSellDocHent() const;
private:
	NullInt GetIndex(const std::wstring& indexName, IPrintProfile* profile);
private:
	int m_recordIndex;
	int m_pageIndex;
	bool m_useSellDocHent;
};


class HerdBook : public DocumentInterface,private KeysInputInterface
{
	class HerdBookDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(HerdBookDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const CowPresenceView* row);
			std::wstring FormatHent(const HentView* pHent);
			std::wstring TranslateMotive(Motive motive) const;
		private:
			HerdBookDS* m_parent;

		};
		
		friend class ItemsBandEvaluator;
	public:
		HerdBookDS(HerdBook* parent,IPrintProfile* profile);
		~HerdBookDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	private:
		IPrintProfile* m_profile;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		HerdBook* m_parent;
		HerdBookOptions m_options;
	};
	
	friend class HerdBookDS;
public:
	HerdBook(bkprint::Template* templ);
	~HerdBook();
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void Feed(SeasonSession* pSession,const UniquePresenceKeys& keys);
	
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual void SetContext(DocumentContext* context);
protected:	
	NullInt GetIndex(const std::wstring& indexName, IPrintProfile* profile);

	
	
private:
	std::vector<CowPresenceView*> m_cows;
	bkprint::Template* m_pTemplate;	
	DocumentContext* m_pContext;
};




#endif
