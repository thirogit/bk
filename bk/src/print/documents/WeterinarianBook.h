#ifndef __WETERINARIANBOOK_H__
#define __WETERINARIANBOOK_H__

#include <vector>
#include "../../bkprint/template/Template.h"
#include "../../view/CowPresenceView.h"
#include "../DocumentInterface.h"
#include "../KeysInputInterface.h"
#include "../../bkprint/renderer/PrintOut.h"
#include <compare/CompareDefs.h>
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"

template<class RecordType>
class RecordComparatorByField
{
public:
	virtual COMPARE_RESULT Compare(const RecordType* record1, const RecordType* record2) = 0;
	virtual std::wstring GetFieldCode() const = 0;
};

class CowPresenceViewCowNoFieldComparator : public RecordComparatorByField<CowPresenceView>
{
public:
	static const std::wstring FIELD_CODE;
	virtual COMPARE_RESULT Compare(const CowPresenceView* record1, const CowPresenceView* record2);
	virtual std::wstring GetFieldCode() const;
};

class CowPresenceViewInDateFieldComparator : public RecordComparatorByField<CowPresenceView>
{
public:
	static const std::wstring FIELD_CODE;

	virtual COMPARE_RESULT Compare(const CowPresenceView* record1, const CowPresenceView* record2);
	virtual std::wstring GetFieldCode() const;
};

class CowPresenceViewOutDateFieldComparator : public RecordComparatorByField<CowPresenceView>
{
public:
	static const std::wstring FIELD_CODE;
	virtual COMPARE_RESULT Compare(const CowPresenceView* record1, const CowPresenceView* record2);
	virtual std::wstring GetFieldCode() const;
};

class RecordComparator
{
public:
	void sort(std::vector<CowPresenceView*>& records);
private:
	std::vector<RecordComparatorByField<CowPresenceView>*> m_fieldComparators;
};

class WetBookOptions
{
public:
	WetBookOptions(IPrintProfile* profile);

	int recordIndex() const;
	
private:
	NullInt GetIndex(const std::wstring& indexName, IPrintProfile* profile);
private:
	int m_recordIndex;
	
};

struct WeterinarianBookRow
{
	HentView* fromPlace;
	HentView* toPlace;
	std::wstring toPlaceStr;
	std::wstring inWetNo;
	std::wstring outWetNo;
	CowPresenceView* cow;
};


class WeterinarianBook : public DocumentInterface,private KeysInputInterface
{
	class WetBookDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(WetBookDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const WeterinarianBookRow* row);
			std::wstring FormatHent(const HentView* hent) const;
			std::wstring FormatCow(const CowPresenceView *pCow);
		private:
			WetBookDS* m_parent;

		};

		friend class ItemsBandEvaluator;
	public:
		WetBookDS(WeterinarianBook* parent, IPrintProfile* profile);
		~WetBookDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	private:
		IPrintProfile* m_profile;
		ItemsBandEvaluator* m_itemsBandEvaluator;
		WeterinarianBook* m_parent;
		WetBookOptions m_options;
	};

	friend class WetBookDS;
public:
	WeterinarianBook(bkprint::Template* templ);
	~WeterinarianBook();

	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);
	virtual void Feed(SeasonSession* pSession,const UniquePresenceKeys& keys);
	virtual  bkprint::PrintOut Print(IPrintProfile* profile);
	virtual void SetContext(DocumentContext* context);
	
protected:
	void DeleteRows();

private:
	std::vector<WeterinarianBookRow*> m_rows;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
	
};

#endif
