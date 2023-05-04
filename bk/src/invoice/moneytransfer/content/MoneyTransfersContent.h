#pragma once
#include "..\..\..\ctrls\contentlistctrl\ListContent.h"
#include "MoneyTransferRow.h"
#include "../MoneyTransferEntry.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>


class MoneyTransfersContent : public ListContent
{
	struct tag_InoiceId{};

	struct RowKeyExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(MoneyTransferRow* pRow) const
		{
			return pRow->Content()->GetInvoiceId();
		}
	};

	typedef boost::multi_index_container
	<
		MoneyTransferRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_InoiceId>, RowKeyExtractor >
										>
	> RowIndexType;	


public:
	MoneyTransfersContent();
	~MoneyTransfersContent();

	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);

	MoneyTransferRow* GetMoneyTransferRow(int row);
	
	void AddRow(const MoneyTransferEntry* row);
	void UpdateRow(const MoneyTransferEntry* rowKey);
	void RemoveRow(uint32_t invoiceId);
	
	void Clear();
	
protected:
	
	

protected:	
	RowIndexType m_Rows;

};


