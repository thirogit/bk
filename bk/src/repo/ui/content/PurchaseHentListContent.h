#pragma once
#include "..\..\..\ctrls\contentlistctrl\ListContent.h"
#include "../content/PurchaseHentRow.h"
#include "../view/PurchaseHentViewCreator.h"


class PurchaseHentListContent : public ListContent
{
	struct tag_Id{};
	
	struct RowKeyExtractor
	{
		typedef std::wstring result_type;
		result_type operator()(PurchaseHentRow* pRow) const
		{
			return pRow->Content()->GetId();
		}
	};
	
	typedef boost::multi_index_container
	<
		PurchaseHentRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_Id>, RowKeyExtractor >
										>
	> RowIndex;

	
public:
	PurchaseHentListContent();
	~PurchaseHentListContent();
	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);

	virtual void SortContent(const SortAttributes& attributes);
			
	void AddRow(IPurchaseHent* purchaseHent);
	void RemoveRow(const std::wstring& hentId);
	void UpdateRow(IPurchaseHent* purchaseHent);
	
	PurchaseHentRow* GetPurchaseHentRowAt(int row);	
	PurchaseHentRow* FindRow(const std::wstring& hentId);

	void Clear();

protected:
	RowIndex m_Rows;	
	PurchaseHentViewCreator m_viewCreator;
};


