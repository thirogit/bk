#pragma once
#include "..\..\..\ctrls\contentlistctrl\ListContent.h"
#include "../content/PurchaseHeaderRow.h"
#include "../view/PurchaseHeaderViewCreator.h"

class PurchaseHeaderListContent : public ListContent
{
	struct tag_Id{};

	template<class from_type, class to_type>
	class cast_ptr : public std::unary_function<typename from_type, typename to_type>
	{
	public:
		typedef std::unary_function<typename from_type, typename to_type> _Base;
		typedef typename _Base::argument_type argument_type;
		typedef typename _Base::result_type result_type;

		result_type operator()(const argument_type& _Right) const
		{
			return (result_type)_Right;
		}

		result_type operator()(argument_type& _Right) const
		{
			return (result_type)_Right;
		}

	protected:

		typename from_type value;
	};
	
	struct RowKeyExtractor
	{
		typedef std::wstring result_type;
		result_type operator()(PurchaseHeaderRow* pRow) const
		{
			return pRow->Content()->GetPurchaseId();
		}
	};
	
	typedef boost::multi_index_container
	<
		PurchaseHeaderRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_Id>, RowKeyExtractor >
										>
	> RowIndex;

	
public:
	PurchaseHeaderListContent();
	~PurchaseHeaderListContent();
	
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);

	virtual void SortContent(const SortAttributes& attributes);
			
	void AddRow(IPurchaseHeader* purchaseHeader);
	void RemoveRow(const std::wstring& purchaseId);
	void UpdateRow(IPurchaseHeader* purchase);
	
	PurchaseHeaderRow* GetPurchaseRowAt(int row);
	PurchaseHeaderRow* FindRow(const std::wstring& purchaseId);

	void Clear();

	template<class T>
	void AddRows(T & collection) 
	{
		typedef T::value_type value_type;
				
		auto transform_it = boost::make_transform_iterator< cast_ptr<value_type, IPurchaseHeader*> >(boost::begin(collection));
		auto it = purchaseheader_iterator(transform_it);

		
		auto end_transform_it = boost::make_transform_iterator< cast_ptr<value_type, IPurchaseHeader*> >(boost::end(collection));
		auto end_it = purchaseheader_iterator(end_transform_it);

		
		AddRows(it, end_it);
	}

	typedef boost::range_detail::any_iterator<IPurchaseHeader*, boost::forward_traversal_tag, IPurchaseHeader*, std::ptrdiff_t > purchaseheader_iterator;

	void AddRows(purchaseheader_iterator beginIt, purchaseheader_iterator endIt);
	
protected:
	RowIndex m_Rows;	
	PurchaseHeaderViewCreator m_viewCreator;
};


