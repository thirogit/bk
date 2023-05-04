#pragma once
#include "..\..\..\ctrls\contentlistctrl\ListContent.h"
#include "../content/PurchaseCowRow.h"
#include "../view/PurchaseCowViewCreator.h"

class PurchaseCowListContent : public ListContent
{
	struct tag_Id{};
	
	struct RowKeyExtractor
	{
		typedef std::wstring result_type;
		result_type operator()(PurchaseCowRow* pRow) const
		{
			return pRow->Content()->GetId();
		}
	};
	
	typedef boost::multi_index_container
	<
		PurchaseCowRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_Id>, RowKeyExtractor >
										>
	> RowIndex;

	
public:
	PurchaseCowListContent();
	~PurchaseCowListContent();
	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);

	virtual void SortContent(const SortAttributes& attributes);
			
	void AddRow(IPurchaseCow* purchaseCow);
	void RemoveRow(const std::wstring& cowId);
	void UpdateRow(IPurchaseCow* purchaseCow);
	
	PurchaseCowRow* GetPurchaseCowRowAt(int row);	
	PurchaseCowRow* FindRow(const std::wstring& cowId);

	void Clear();

protected:
	RowIndex m_Rows;	
	PurchaseCowViewCreator m_viewCreator;
};


