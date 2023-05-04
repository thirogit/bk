#ifndef __INVOICETRANSACTIONSKELETON_H__
#define __INVOICETRANSACTIONSKELETON_H__

#include "ITransactionInterface.h"
#include "ObserverableTransaction.h"
#include "TransactionState.h"
#include <data\impl\InvoiceDetails.h>
#include "InvoiceItem.h"
#include <data\runtime\InvoiceItemKey.h>
#include <data\impl\CowDetails.h>
#include <data\impl\CowInvoiceEntryDetails.h>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <data\runtime\InvoiceTransactionItemTuple.h>
#include <data\impl\InvoiceHent.h>

class InvoiceTransactionSkeleton : public ObserverableTransaction
{
	struct tag_itemId{};
	//struct tag_itemKey{};
	struct tag_cowId{};
	struct tag_docId{};

	struct ItemIdKeyExtractor
	{
		typedef ItemId result_type;
		result_type operator()(InvoiceItem* item) const 
		{
			return item->GetItemKey().GetItemId();
		}
	};

	struct CowIdKeyExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(InvoiceItem* item) const 
		{
			return item->GetCowId();
		}
	};

	struct DocIdKeyExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(InvoiceItem* item) const 
		{
			return item->GetItemKey().GetDocKey().GetDocNo();
		}
	};

	typedef boost::multi_index_container
	<
		InvoiceItem*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_itemId>, ItemIdKeyExtractor >,
										//boost::multi_index::ordered_unique < boost::multi_index::tag<tag_itemKey>, boost::multi_index::const_mem_fun<InvoiceItem,const InvoiceItemKey&, &InvoiceItem::GetItemKey> >,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_cowId>, CowIdKeyExtractor >,
										boost::multi_index::ordered_non_unique < boost::multi_index::tag<tag_docId>, DocIdKeyExtractor >
										>
	> InvoiceItemIndex;


public:
	BuyInvoiceDocTransaction(ITransactionInterface* pInterface);
	~BuyInvoiceDocTransaction();
		
	const InvoiceDetails& GetDetails() const;
	bool IsUpdating() const;		
	void UpdateDetails(const InvoiceDetails& details);
	
	const IInvoiceHent* GetHent() const;
	void SetHent(uint32_t hentId);
	void UpdateCow(ItemId id,const CowDetails& details);
	void UpdateEntry(ItemId id,const CowInvoiceEntryDetails& details);	
	const InvoiceItem* GetItemAt(int index);
	const InvoiceItem* GetItem(ItemId id);
	size_t GetItemCount() const;
		
	void Commit();
	void Rollback();
	
protected:
	void ThrowIfNotOpen() const;
	InvoiceItem* CreateItemOutOf(const InvoiceTransactionItemTuple& itemTuple);
	InvoiceItem* FindItem(ItemId id);
protected:
	InvoiceHent m_hent;
	InvoiceDetails m_details;
	TransactionState m_State;
	ITransactionInterface* m_pInterface;		
	uint32_t m_invoiceId;	
	InvoiceItemIndex m_items;	
	InvoiceTransactionFacadeImpl* m_pFacade;
};



#endif