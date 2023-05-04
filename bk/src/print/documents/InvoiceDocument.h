#ifndef __INVOICEDOCUMENT_H__
#define __INVOICEDOCUMENT_H__

#include "../DocumentInterface.h"
#include "../../view/HentView.h"
#include "../../bkprint/template/Template.h"
#include <types\NullInt.h>
#include <types\NullDecimal.h>
#include <boost\range\any_range.hpp>
#include "../IdsInputInterface.h"
#include "../DocumentPredicate.h"
#include "../../bkprint/renderer/ExpandableTemplateDataSource.h"
#include <mustache\Mustache.h>

class InvoiceOptions
{
public:
	InvoiceOptions(IPrintProfile* profile);

	const std::wstring& title() const;
	const DecimalFormat& priceFormat() const;
	const DecimalFormat& weightFormat() const;
	const DecimalFormat& pricePerKgFormat() const;
	const std::wstring& item() const;
	const std::wstring& extras() const;
	const std::wstring& goodsCategory() const;
	const std::wstring& goodsDeclination1() const;
	const std::wstring& goodsDeclination2_4() const;
	const std::wstring& goodsDeclination4plus() const;
private:
	std::wstring m_title;
	DecimalFormat m_priceFormat;
	DecimalFormat m_weightFormat;
	DecimalFormat m_pricePerKgFormat;
	std::wstring m_item;
	std::wstring m_extras;
	std::wstring m_goodsCategory;
	std::wstring m_goodsDeclination1;
	std::wstring m_goodsDeclination2_4;
	std::wstring m_goodsDeclination4plus;
};

class InvoiceTypePredicate : public DocumentPredicate
{
public:
	InvoiceTypePredicate(InvoiceType type);
	virtual bool applies(const InvoiceView* pView);
private:
	InvoiceType m_type;
};

class InvoiceItemRow
{
public:
	InvoiceItemRow();
	InvoiceItemRow(const InvoiceItemRow& src);
	InvoiceItemRow& operator=(const InvoiceItemRow& right);
	void CopyFrom(const InvoiceItemRow& src);

	void SetDescription(const std::wstring& sDesc);
	const std::wstring& GetDescription() const;

	void SetQty(const Decimal& weight);
	void SetQty(int pieces);

	NullDecimal GetQtyWeight() const;
	NullInt GetQtyPieces() const;
	
	QtyUnit GetQtyUnit() const;

	void SetNetTotal(const Decimal& netTotal);
	const Decimal& GetNetTotal() const;

	void SetVatTotal(const Decimal& vatTotal);
	const Decimal& GetVatTotal() const;

	NullDecimal GetUnitPrice() const;
protected:
	NullDecimal GetQty() const;

private:
	std::wstring m_sDesc;
	NullInt m_qtyPiece;
	NullDecimal m_qtyWeight;
	QtyUnit m_qtyUnit;
	Decimal m_netTotal;
	Decimal m_vatTotal;
};


struct CowClassTuple
{
	std::wstring name;
	std::wstring code;
};

class InvoiceItem
{
public:
	CowNo cowNo;
	CowClassTuple cowClass;
	CowSex sex;
	Decimal weight;
	Decimal netValue;
	Decimal taxValue;
	Decimal grossValue;	
};


class DeductionItem
{
public:
	std::wstring m_code;
	std::wstring m_reason;	
	Decimal m_fraction;
	Decimal m_amount;
};


typedef std::vector<InvoiceItem*> InvoiceItems;
typedef std::vector<DeductionItem*> Deductions;

class HentRecord
{
public:
	std::wstring name;
	std::wstring street;
	std::wstring pobox;
	std::wstring city;
	std::wstring zip;
	std::wstring number;
	std::wstring fiscalno;
	std::wstring personalno;
	std::wstring personalidno;
	std::wstring personalidissuedt;
	std::wstring personalidissuepost;
	std::wstring bankname;
	std::wstring accountnumber;

	void CopyFrom(const IFarm* farm);
	void CopyFrom(const IInvoiceHent* hent);
};


class InvoiceRecord
{
public:
	InvoiceRecord();

	HentRecord buyer;
	HentRecord seller;
	std::wstring invoiceNo;
	DateTime invoiceDate;
	DateTime transactionDate;
	std::wstring transactionPlace;
	std::wstring extras;

	Decimal totalNet;
	Decimal totalGrossAfterDeductions;
	Decimal totalVat;
	Decimal totalGross;
	Decimal totalDeductedAmount;
	Decimal vatRate;
	int payduedays;
	PayWay payway;	

	InvoiceItems items;
	Deductions deductions;
};


class InvoiceItemsGroupByClass
{
public:	
	CowClassTuple cowClass;
	int qty;
	Decimal weight;
	Decimal price;
};

typedef boost::any_range<IInvoice*, boost::forward_traversal_tag, IInvoice*, std::ptrdiff_t> InvoiceRange;
typedef boost::any_range<const InvoiceRecord*, boost::forward_traversal_tag, InvoiceRecord* const, std::ptrdiff_t> InvoiceRecordRange;


class ProductNumeralDeclination
{
public:
	ProductNumeralDeclination(InvoiceOptions* options);
	std::wstring Declinate(int qty) const;
private:
	InvoiceOptions* m_options;
};

class InvoiceEntryVariables
{
public:
	InvoiceEntryVariables(InvoiceOptions* options, DocumentContext* context);

	MustacheVariable ProductNameVariable(int qty) const;
	MustacheVariable ProductQtyVariable(int qty) const;
	MustacheVariable CowNoVariable(const CowNo& cowNo) const;
	MustacheVariable CowSexVariable(const CowSex& sex) const;
	MustacheVariable CowClassCodeVariable(const std::wstring& classCd) const;
	MustacheVariable CowClassNameVariable(const std::wstring& classNm) const;
	MustacheVariable TotalWeight(const Decimal& weight) const;

private:
	InvoiceOptions* m_options;
	DocumentContext* m_context;
	ProductNumeralDeclination m_declination;
};

class InvoiceInputInterface : public IdsInputInterface
{
public:
	~InvoiceInputInterface();
	InvoiceRecordRange GetRecords();
	bool Query(const type_info& interfase_type);
	virtual void Feed(SeasonSession* pSession, const UniqueIds& range);
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range) = 0;
	void AddRecord(InvoiceRecord* record);
	void DeleteRecords();
private:
	std::vector<InvoiceRecord*> m_records;
};

class GenericInvoiceDocument : public DocumentInterface
{
	class InvoiceDocumentDS : public bkprint::ExpandableTemplateDataSource
	{
		class ItemsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			ItemsBandEvaluator(InvoiceDocumentDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const InvoiceItemRow* row);
			std::wstring GetQtyUnit(const InvoiceItemRow *pRow) const;
			std::wstring GetQty(const InvoiceItemRow *pRow) const;
		private:
			InvoiceDocumentDS* m_parent;			
		};

		class DeductionsBandEvaluator : public bkprint::BandEvaluator
		{
		public:
			DeductionsBandEvaluator(InvoiceDocumentDS* parent);
			virtual bkprint::Values Header();
			virtual bkprint::Values Footer();
			virtual size_t Count() const;
			virtual bkprint::Values At(int index);

		protected:
			bkprint::Values CreateRowValues(size_t nindex, const DeductionItem* row);
		private:
			InvoiceDocumentDS* m_parent;
		};


		friend class ItemsBandEvaluator;
	public:
		InvoiceDocumentDS(const InvoiceRecord* record,
						  InvoiceOptions* options,
						  GenericInvoiceDocument* parent);
		~InvoiceDocumentDS();
		virtual bkprint::Values GetHeaderValues();
		virtual bkprint::Values GetFooterValues();
		virtual bkprint::Values GetPageHeaderValues(int pageCount, int page);
		virtual bkprint::Values GetPageFooterValues(int pageCount, int page);
		virtual bkprint::BandEvaluator* GetBandValues(const std::wstring& bandName);
	private:		
		std::wstring FormatHentAddress(const HentRecord* hent);
		GenericInvoiceDocument* m_parent;
		const InvoiceRecord* m_record;
		std::vector<InvoiceItemRow> m_rows;		
		ItemsBandEvaluator* m_itemsBandEvaluator;
		DeductionsBandEvaluator* m_deductionsBandEvaluator;
		InvoiceOptions* m_options;
	};

	class HyperInvoiceDocumentDS : public bkprint::HyperExpandableTemplateDataSource
	{
	public:
		HyperInvoiceDocumentDS(InvoiceRecordRange records, InvoiceOptions* options, GenericInvoiceDocument* parent);
		~HyperInvoiceDocumentDS();
		virtual int GetDSCount();
		virtual bkprint::ExpandableTemplateDataSource* GetDS(int index);
	private:
		std::vector<InvoiceDocumentDS*> m_dataSources;
	};

public:
	GenericInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface);
	virtual bool QueryInputInterface(const type_info& interfase_type,void** interfase);	
	virtual void SetContext(DocumentContext* context);	
	virtual bkprint::PrintOut Print(IPrintProfile* profile);
	virtual ~GenericInvoiceDocument();
protected:
	virtual std::vector<InvoiceItemRow> GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options) = 0;
protected:
	InvoiceInputInterface* m_pInputInterface;
	bkprint::Template* m_pTemplate;
	DocumentContext* m_pContext;
};


class DetailedPieceInvoiceDocument : public GenericInvoiceDocument
{
public:
	DetailedPieceInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface);
protected:
	virtual std::vector<InvoiceItemRow> GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options);
};


class DetailedWeightInvoiceDocument : public GenericInvoiceDocument
{
public:
	DetailedWeightInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface);
protected:
	virtual std::vector<InvoiceItemRow> GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options);
};

class ClassPieceInvoiceDocument : public GenericInvoiceDocument
{
public:
	ClassPieceInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface);
protected:
	virtual std::vector<InvoiceItemRow> GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options);
	
};

class ClassWeightInvoiceDocument : public GenericInvoiceDocument
{
public:
	ClassWeightInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface);
protected:
	virtual std::vector<InvoiceItemRow> GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options);
};


class CompactPieceInvoiceDocument : public GenericInvoiceDocument
{
public:
	CompactPieceInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface);
protected:
	virtual std::vector<InvoiceItemRow> GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options);
};

class CompactWeightInvoiceDocument : public GenericInvoiceDocument
{
public:
	CompactWeightInvoiceDocument(bkprint::Template* templ, InvoiceInputInterface* inputInterface);
protected:
	virtual std::vector<InvoiceItemRow> GetRowsForInvoiceRecord(const InvoiceRecord* record, InvoiceOptions* options);
};

class BuyInvoiceInputInterface : public InvoiceInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};

class SellInvoiceInputInterface : public InvoiceInputInterface
{
protected:
	virtual void Consume(SeasonSession* pSession, const UniqueIds& range);
};

template<class BASE>
class BuyInvoiceDocument : public BASE
{
public:
	BuyInvoiceDocument(bkprint::Template* templ) : BASE(templ, &m_input) {}
private:
	BuyInvoiceInputInterface m_input;
};

template<class BASE>
class SellInvoiceDocument : public BASE
{
public:
	SellInvoiceDocument(bkprint::Template* templ) : BASE(templ, &m_input) {}
private:
	SellInvoiceInputInterface m_input;
};

typedef  BuyInvoiceDocument < DetailedPieceInvoiceDocument > BuyDetailedPieceInvoiceDocument;
typedef  SellInvoiceDocument < DetailedPieceInvoiceDocument > SellDetailedPieceInvoiceDocument;

typedef  BuyInvoiceDocument < ClassPieceInvoiceDocument > BuyClassPieceInvoiceDocument;
typedef  SellInvoiceDocument < ClassPieceInvoiceDocument > SellClassPieceInvoiceDocument;

typedef  BuyInvoiceDocument < ClassWeightInvoiceDocument > BuyClassWeightInvoiceDocument;
typedef  SellInvoiceDocument < ClassWeightInvoiceDocument > SellClassWeightInvoiceDocument;

typedef  BuyInvoiceDocument < CompactPieceInvoiceDocument > BuyCompactPieceInvoiceDocument;
typedef  SellInvoiceDocument < CompactPieceInvoiceDocument > SellCompactPieceInvoiceDocument;

typedef  BuyInvoiceDocument < CompactWeightInvoiceDocument > BuyCompactWeightInvoiceDocument;
typedef  SellInvoiceDocument < CompactWeightInvoiceDocument > SellCompactWeightInvoiceDocument;

typedef  BuyInvoiceDocument < DetailedWeightInvoiceDocument > BuyDetailedWeightInvoiceDocument;

#endif
