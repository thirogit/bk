#ifndef __INVOICECONTENT_H__
#define __INVOICECONTENT_H__

#include "CowInvoiceEntry.h"
#include "../../arrays/Enumerator.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include  <casajson/json.h>

class SerializableInvoiceContent;

class InvoiceContent
{
	struct index_by_id{};

	typedef boost::multi_index_container
	<
	  CowInvoiceEntry*,
	  boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<index_by_id>, boost::multi_index::const_mem_fun<CowInvoiceEntry,uint32_t, &CowInvoiceEntry::GetCowId> >										
									>
	> ContentIndex;

public:
	InvoiceContent();
	~InvoiceContent();
		
	int GetCount() const;
	PtrEnumerator<ICowInvoiceEntry> EnumEntries() const;
	PtrEnumerator<CowInvoiceEntry> EnumEntries();
	CowInvoiceEntry* GetEntry(uint32_t cowId) const;
	CowInvoiceEntry* GetEntryAt(int index) const;
	void RemoveAt(int index);
	void RemoveAll();
	CowInvoiceEntry* AddEntry(uint32_t cowId);
	void AddEntry(CowInvoiceEntry* cowEntry);
	void RemoveEntry(uint32_t cowId);

	static InvoiceContent FromJson(const json::value& value);
	json::value ToJson() const;

protected:
	ContentIndex m_Entries;

	friend class SerializableInvoiceContent;
};

#endif