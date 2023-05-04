#ifndef __TRANSACTIONPASSPORTCOLLECTION_H__
#define __TRANSACTIONPASSPORTCOLLECTION_H__

#include <boost/iterator/iterator_facade.hpp>
#include "../passportsegregation/CowPassport.h"
#include "../passportsegregation/PassportCollection.h"

class transaction_iterator
  : public boost::iterator_facade<
        transaction_iterator
		, CowPassport
      , boost::forward_traversal_tag
	  ,CowPassport
    >
{
 public:
    transaction_iterator() : m_transactionFacade(NULL),m_position(-1) {}

    explicit transaction_iterator(DocTransactionFacade* transactionFacade,int position)
		: m_transactionFacade(transactionFacade),m_position(position)
    {}

 private:	 
    friend class boost::iterator_core_access;

    void increment() { m_position++; }

    bool equal(transaction_iterator const& other) const
    {
        return this->m_transactionFacade == other.m_transactionFacade && this->m_position == other.m_position;
    }

    CowPassport dereference() const 
	{ 
		const DocItem* pItem = m_transactionFacade->GetItemAt(m_position);
		const CowDetails& details = pItem->GetCowDetails();
		const CowEntryDetails& entryDetails = pItem->GetCowEntry();
		return CowPassport(details.GetCowNo(),entryDetails.GetGroup(),details.GetPassNo());
	}
     
	 DocTransactionFacade* m_transactionFacade;
	 int m_position;
};

class TransactionPassportCollection : public PassportCollection
{
public:
	TransactionPassportCollection(DocTransactionFacade* facade) : m_facade(facade)
	{}

	virtual CowPassportRange Enum() const
	{
		return CowPassportRange(transaction_iterator(m_facade,0),transaction_iterator(m_facade,m_facade->GetItemCount()));
	}
private:
	DocTransactionFacade* m_facade;
};

#endif
