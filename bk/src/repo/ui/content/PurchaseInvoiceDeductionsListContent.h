#pragma once
#include "..\..\..\ctrls\contentlistctrl\ListContent.h"
#include "../content/PurchaseInvoiceDeductionRow.h"
#include "../view/PurchaseInvoiceDeductionViewCreator.h"


class PurchaseInvoiceDeductionsListContent : public ListContent
{
	struct tag_Id{};
	
	struct RowKeyExtractor
	{
		typedef std::wstring result_type;
		result_type operator()(PurchaseInvoiceDeductionRow* pRow) const
		{
			return pRow->Content()->GetId();
		}
	};
	
	typedef boost::multi_index_container
	<
		PurchaseInvoiceDeductionRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_Id>, RowKeyExtractor >
										>
	> RowIndex;

	
public:
	PurchaseInvoiceDeductionsListContent();
	~PurchaseInvoiceDeductionsListContent();
	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);

	virtual void SortContent(const SortAttributes& attributes);
			
	void AddRow(IPurchaseInvoiceDeduction* deduction);
	void RemoveRow(const std::wstring& deductionId);
	void UpdateRow(IPurchaseInvoiceDeduction* deduction);
	
	PurchaseInvoiceDeductionRow* GetDeductionRowAt(int row);
	PurchaseInvoiceDeductionRow* FindRow(const std::wstring& deductionId);

	void Clear();

protected:
	RowIndex m_Rows;	
	PurchaseInvoiceDeductionViewCreator m_viewCreator;
};


