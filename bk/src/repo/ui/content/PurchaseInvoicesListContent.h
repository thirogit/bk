#pragma once
#include "..\..\..\ctrls\contentlistctrl\ListContent.h"
#include "../content/PurchaseInvoiceRow.h"
#include "../view/PurchaseInvoiceViewCreator.h"

class PurchaseInvoicesListContent : public ListContent
{
	struct tag_Id{};
	
	struct RowKeyExtractor
	{
		typedef std::wstring result_type;
		result_type operator()(PurchaseInvoiceRow* pRow) const
		{
			return pRow->Content()->GetInvoiceId();
		}
	};
	
	typedef boost::multi_index_container
	<
		PurchaseInvoiceRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_Id>, RowKeyExtractor >
										>
	> RowIndex;

	
public:
	PurchaseInvoicesListContent();
	~PurchaseInvoicesListContent();
	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);

	virtual void SortContent(const SortAttributes& attributes);
			
	void AddRow(IPurchaseInvoice* invoice);
	void RemoveRow(const std::wstring& invoiceId);
	void UpdateRow(IPurchaseInvoice* invoice);
	
	PurchaseInvoiceRow* GetInvoiceRowAt(int row);
	PurchaseInvoiceRow* FindRow(const std::wstring& invoiceId);

	void Clear();

	template<class T>
	void AddRows(T & collection) 
	{
		purchase_iterator beginIt(boost::begin(collection)), endIt(boost::end(collection));
		AddRows(beginIt, endIt);
	}

	typedef boost::range_detail::any_iterator<IPurchaseInvoice*, boost::forward_traversal_tag, IPurchaseInvoice*, std::ptrdiff_t > invoice_iterator;

	void AddRows(invoice_iterator beginIt, invoice_iterator endIt);
	
protected:
	RowIndex m_Rows;	
	PurchaseInvoiceViewCreator m_viewCreator;
};


