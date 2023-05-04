#ifndef __INVOICEDEDUCTIONSCONTENT_H__
#define __INVOICEDEDUCTIONSCONTENT_H__

#include "../view/DeductionElementView.h"
#include "../calculation/CalculationObserver.h"
#include "../../ctrls/contentlistctrl/ListContent.h"
#include "DeductionElementRow.h"
#include "../../context/SeasonSession.h"
#include "../view/DeductionElementViewCreator.h"

#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost\function.hpp>

class InvoiceDeductionsContent : public ListContent, protected CalculationObserver
{
	struct tag_itemId{};

	struct RowKeyExtractor
	{
		typedef ItemId result_type;
		result_type operator()(DeductionElementRow* pRow) const 
		{
			return pRow->Content()->GetItemId();
		}
	};

	typedef boost::multi_index_container
	<
		DeductionElementRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_itemId>, RowKeyExtractor >
										>
	> RowIndex;


public:
	InvoiceDeductionsContent();
	~InvoiceDeductionsContent();
	void Create(InvoiceCalculation* pCalculation,SeasonSession* pSession);
	void Destroy();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	DeductionElementRow* GetDeductionRowAt(int row);
protected:
	DeductionElementRow* FindRow(ItemId id);
	void AddRow(const DeductionElement* deduction);
	void RefreshAllRows();

	virtual void ElementChanged(const CalculationElement* pElement);
	virtual void ElementDeleted(ItemId id);
	virtual void ElementAdded(const CalculationElement* pElement);

	virtual void DeductionChanged(const DeductionElement* pDeduction);
	virtual void DeductionDeleted(ItemId id);
	virtual void DeductionAdded(const DeductionElement* pDeduction);

private:
	InvoiceCalculation* m_pCalculation;
	SeasonSession*		m_pSession;
	RowIndex			m_rows;
};


#endif