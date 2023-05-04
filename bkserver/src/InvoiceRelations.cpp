#include "stdafx.h"
#include "InvoiceRelations.h"
#include <boost/iterator/transform_iterator.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>

InvoiceRelations::FindRelationPredicate::FindRelationPredicate(const DocKey& key) : m_key(key)
{
}

bool InvoiceRelations::FindRelationPredicate::operator()(std::pair<uint32_t,DocKey> element)
{
	return element.second == m_key;
}
	
//----------------------------------------
void InvoiceRelations::AddRelationship(uint32_t invoiceId,const DocKey& key)
{
	m_Relations.insert(std::make_pair(invoiceId,key));
}

void InvoiceRelations::RemoveRelationship(uint32_t invoiceId,const DocKey& key)
{
	auto range = m_Relations.equal_range(invoiceId);
	auto elementIt = std::find_if(range.first,range.second,FindRelationPredicate(key));
	
	if(elementIt != range.second)
		m_Relations.erase(elementIt);
}

std::vector<DocKey> InvoiceRelations::GetInvoiceRelations(uint32_t invoiceId)
{
	return GetInvoiceRelationsOf(invoiceId,DocType_None);
}

std::vector<DocKey> InvoiceRelations::GetInvoiceRelationsOf(uint32_t invoiceId,DocType type)
{

	std::vector<DocKey> result;
	boost::function<bool (DocKey)> filterFn = [type](DocKey key) -> bool
											{
												return (type == DocType_None) || (key.GetDocType() == type);
											};

	auto range = m_Relations.equal_range(invoiceId);

	boost::transform_iterator<boost::function<DocKey (std::pair<uint32_t,DocKey>)>,std::multimap<uint32_t,DocKey>::iterator> it,endIt;

	boost::function<DocKey (std::pair<uint32_t,DocKey>)> f =  [](std::pair<uint32_t,DocKey> pair) -> DocKey { return pair.second; };
	
	it = boost::make_transform_iterator(range.first, f);
	endIt = boost::make_transform_iterator(range.second, f);
	
	while(it != endIt)
	{
		if(filterFn(*it))
			result.push_back(*it);
		it++;
	}

	return std::move(result);
}

bool InvoiceRelations::AreInRelationship(uint32_t invoiceId,const DocKey& key)
{
	auto range = m_Relations.equal_range(invoiceId);
	auto elementIt = std::find_if(range.first,range.second,FindRelationPredicate(key));
	
	return (elementIt != range.second);
}

void InvoiceRelations::RemoveRelations(uint32_t invoiceId,const std::vector<DocKey>& keys)
{
	BOOST_FOREACH(DocKey key, keys)
	{
		RemoveRelationship(invoiceId,key);
	}
}

DocKey InvoiceRelations::GetInvoiceRelationship(uint32_t invoiceId)
{
	std::vector<DocKey> keys = GetInvoiceRelations(invoiceId);

	if(!keys.empty())
	{
		return keys[0];
	}
	return DocKey();
}
