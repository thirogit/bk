#ifndef __INVOICECALCULATIONCONTENT_H__
#define __INVOICECALCULATIONCONTENT_H__

#include "../view/CalculationElementView.h"
#include "../calculation/EmptyCalculationObserver.h"
#include "../../ctrls/contentlistctrl/ListContent.h"
#include "CalculationElementRow.h"
#include "../../context/SeasonSession.h"
#include "../view/CalculationViewCreator.h"

#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost\function.hpp>

typedef boost::function<CalculationViewCreator* (SeasonSession*&)> CalculationElementViewFactory;


class InvoiceCalculationContent : public ListContent, protected EmptyCalculationObserver
{
	struct tag_itemId{};

	struct RowKeyExtractor
	{
		typedef ItemId result_type;
		result_type operator()(CalculationElementRow* pRow) const 
		{
			return pRow->Content()->GetItemId();
		}
	};

	typedef boost::multi_index_container
	<
		CalculationElementRow*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_itemId>, RowKeyExtractor >
										>
	> RowIndex;


public:
	InvoiceCalculationContent(CalculationElementViewFactory& factory);
	~InvoiceCalculationContent();
	void Create(InvoiceCalculation* pCalculation,SeasonSession* pSession);
	void Destroy();
	virtual int GetRowCount() const;
	virtual ListRow* GetRow(int row);
	virtual void SortContent(const SortAttributes& attributes);
	CalculationElementRow* GetCalculationRow(int row);
protected:
	CalculationElementRow* FindRow(ItemId id);

	virtual void ElementChanged(const CalculationElement* pElement);
	virtual void ElementDeleted(ItemId id);
	virtual void ElementAdded(const CalculationElement* pElement);

private:
	InvoiceCalculation* m_pCalculation;
	SeasonSession*		m_pSession;
	RowIndex			m_rows;
	CalculationElementViewFactory m_factory;
};


#endif