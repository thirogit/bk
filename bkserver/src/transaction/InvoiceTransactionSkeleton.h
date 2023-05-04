#pragma once

#include <data/impl/InvoiceDetails.h>
#include <data/impl/InvoiceHent.h>
#include "InvoiceTransactionCowItem.h"
#include <data/runtime/ItemId.h>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/range/any_range.hpp>
#include "ISeasonInterface.h"
#include <data/runtime/IItem.h>
#include <data/runtime/ItemIndex.h>
#include <data/impl/InvoiceDeduction.h>
#include "DeductionTransactionItem.h"

class InvoiceTransactionSkeleton
{
protected:

	struct tag_itemId{};
	struct tag_itemKey{};
	struct tag_cowId{};
	struct tag_docId{};

	struct ItemIdKeyExtractor
	{
		typedef ItemId result_type;
		result_type operator()(InvoiceTransactionCowItem* item) const 
		{
			return item->GetItemKey().GetItemId();
		}
	};

	struct CowIdKeyExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(InvoiceTransactionCowItem* item) const 
		{
			return item->GetCowId();
		}
	};

	struct DocIdKeyExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(InvoiceTransactionCowItem* item) const 
		{
			return item->GetItemKey().GetDocKey().GetDocNo();
		}
	};

	typedef boost::multi_index_container
	<
		InvoiceTransactionCowItem*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_itemId>, ItemIdKeyExtractor >,
										//boost::multi_index::ordered_unique < boost::multi_index::tag<tag_itemKey>, boost::multi_index::const_mem_fun<InvoiceTransactionCowItem,const InvoiceItemKey&, &InvoiceTransactionCowItem::GetItemKey> >,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_cowId>, CowIdKeyExtractor >,
										boost::multi_index::ordered_non_unique < boost::multi_index::tag<tag_docId>, DocIdKeyExtractor >
										>
	> InvoiceTransactionItemsIndex;

		
	typedef boost::any_range<const InvoiceTransactionCowItem*,boost::forward_traversal_tag, InvoiceTransactionCowItem* const, std::ptrdiff_t> ItemsRange;

	typedef ItemIndex<DeductionTransactionItem> DeductionsIndex;

public:
	InvoiceTransactionSkeleton(ISeasonInterface* pInterface,uint32_t transactionId);	
	uint32_t GetTransactionId() const;

protected:	
	void SetHent(const InvoiceHent& hent);
	InvoiceTransactionCowItem* FindItem(const ItemId& itemId);
	InvoiceTransactionCowItem* AcquireItem(const ItemId& itemId);
	void DeleteItem(const ItemId& itemId);
	void DeleteDocItems(uint32_t docId);


	DeductionTransactionItem* FindDeduction(const ItemId& itemId);
	ItemId AddDeduction(const InvoiceDeduction& deduction);
	ItemId AddDeduction(const IInvoiceDeduction* deduction);
	void UpdateDeduction(const ItemId& itemId, const InvoiceDeduction& deduction);
	void DeleteDeduction(const ItemId& itemId);
		
	template<typename F> void Do(F const& f)
	{
		std::function<void (SeasonSessionFacade*)> fn = f;
		m_pInterface->Do(fn);
	}

	template<typename R,typename F>
	R Eval(F const & functor)
	{
		std::function<R (SeasonSessionFacade*)> evalFunctor = functor;
		return m_pInterface->Eval(evalFunctor);
		
	}

	template<typename F> void UserDo(F const& f)
	{
		std::function<void(UserSessionFacade*)> fn = f;
		m_pInterface->DoUser(fn);
	}

	template<typename R, typename F>
	R EvalUser(F const & functor)
	{
		std::function<R(UserSessionFacade*)> evalFunctor = functor;
		return m_pInterface->EvalUser(evalFunctor);
	}


	ItemId ObtainNewItemId();
	virtual void Cleanup();

protected:
	InvoiceDetails m_details;
	InvoiceHent m_hent;
	uint32_t m_invoiceId;
	uint32_t m_invoiceNo;
	InvoiceType m_type;
	InvoiceTransactionItemsIndex m_Items;
	DeductionsIndex m_Deductions;
private:
	uint32_t m_transactionId;
	ISeasonInterface* m_pInterface;
	boost::uuids::basic_random_generator<boost::mt19937> m_ItemIdGenerator;
};
