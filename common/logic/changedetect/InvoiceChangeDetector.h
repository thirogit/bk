#pragma once

#include "../../data/IInvoice.h"
#include <vector>
#include <cstdint>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include "ChangeEvent.h"

class InvoiceChange
{
public:
	InvoiceChange(ChangeEvent event,uint32_t cowId,ICowInvoiceEntry* pOld,ICowInvoiceEntry* pNew);
	InvoiceChange(const InvoiceChange& src);

	uint32_t GetCowId() const;
	ICowInvoiceEntry* Old() const;
	ICowInvoiceEntry* New() const;
	ChangeEvent Event() const;

private:
	uint32_t m_cowId;
	ICowInvoiceEntry* m_pOld;
	ICowInvoiceEntry* m_pNew;
	ChangeEvent  m_event;	
};

class InvoiceChanges
{
public:
	typedef std::vector<InvoiceChange*> change_vector;
	typedef change_vector::const_iterator const_iterator;
	typedef change_vector::iterator iterator;
	
	InvoiceChanges();
	~InvoiceChanges();
	InvoiceChanges(InvoiceChanges&& changes);
	InvoiceChanges(const InvoiceChanges& changes);

	void add(ChangeEvent event,uint32_t cowId,ICowInvoiceEntry* pOld,ICowInvoiceEntry* pNew);	

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();
	
private:
	change_vector m_changes;
	
};

class InvoiceChangeDetector
{
	struct index_tag_cowid{};

	typedef boost::multi_index_container
	<
		ICowInvoiceEntry*,
		boost::multi_index::indexed_by<	boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_cowid>, boost::multi_index::const_mem_fun<ICowInvoiceEntry,uint32_t, &ICowInvoiceEntry::GetCowId> > >
	> InvoiceEntriesIndex;

public:
	InvoiceChangeDetector(IInvoice* pOld,IInvoice* pNew);
	InvoiceChanges Detect();
private:
	IInvoice* m_pOldInvoice;
	IInvoice* m_pNewInvoice;
};