#pragma once

#include <data/impl/DocDetails.h>
#include <data/impl/CowDetails.h>
#include <data/impl/CowEntryDetails.h>
#include <data/runtime/ItemIndex.h>
#include <boost/uuid/uuid_generators.hpp>
#include <CommonTypes.h>
#include "../SeasonSessionFacade.h"
#include "../ClientSeasonSession.h"
#include "../SeasonSession.h"
#include "ISeasonInterface.h"
#include "DocTransactionCowItem.h"
#include "../log/LogInterface.h"
#include "../log/LoggerAware.h"

class DocTransactionSkeleton :  public LoggerAware
{
public:
	DocTransactionSkeleton(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId);
	~DocTransactionSkeleton();	
	
public: //gcc bug: Lambda can't access private members - https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58972
	ItemId AddCow(uint32_t cowId,const CowDetails& details);	
	void DeleteCow(ItemId itemId);
	
protected:
	uint32_t GetTransactionId() const;
	
	void SetDocDetails(const DocDetails& details);
	const DocDetails& GetDetails();	
	
	void UpdateEntry(ItemId itemId, const CowEntryDetails& details);	
	count_t GetItemsCount() const;
	DocTransactionCowItem* FindItem(ItemId itemId);
	
	void ThrowItemNotFound(ItemId itemId);
	ItemId ObtainNewItemId();
	DocTransactionCowItem* GetItemAt(int index);
	void Clear();	

	template<typename F> void Do(F const & f)
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

	DocTransactionCowItem* FindItem(std::function<bool (DocTransactionCowItem*)>& predicate);
	
	template<typename PREDICATE>
	DocTransactionCowItem* FindItem(PREDICATE const & predicate)
	{
		std::function<bool (DocTransactionCowItem*)> predicateFn = predicate;
		return FindItem(predicateFn);
	}

protected:
	uint32_t m_TransactionId;
	DocDetails m_docDetails;
	ItemIndex<DocTransactionCowItem> m_Items;	
	boost::uuids::basic_random_generator<boost::mt19937> m_ItemIdGenerator;
	ISeasonInterface* m_pInterface;
};
