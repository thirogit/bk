#pragma once

#include <boost/range/any_range.hpp>
#include <map>
#include <data/DocKey.h>
#include <vector>

//typedef boost::any_range<DocKey,boost::forward_traversal_tag, DocKey&, std::ptrdiff_t> DocKeys;
typedef std::multimap<uint32_t,DocKey> RelationsMap;

class InvoiceRelations
{
	class FindRelationPredicate
	{
	public:
		FindRelationPredicate(const DocKey& key);
		bool operator()(std::pair<uint32_t,DocKey> element);
	private:
		DocKey m_key;
	};
public:
	void AddRelationship(uint32_t invoiceId,const DocKey& key);
	void RemoveRelationship(uint32_t invoiceId,const DocKey& key);	
	std::vector<DocKey> GetInvoiceRelations(uint32_t invoiceId);
	std::vector<DocKey> GetInvoiceRelationsOf(uint32_t invoiceId,DocType type);
	bool AreInRelationship(uint32_t invoiceId,const DocKey& key);

	void RemoveRelations(uint32_t invoiceId,const std::vector<DocKey>& keys);
	DocKey GetInvoiceRelationship(uint32_t invoiceId);
private:

	RelationsMap::iterator FindRelation(uint32_t invoiceId,const DocKey& key);
private:
	RelationsMap m_Relations;
};
