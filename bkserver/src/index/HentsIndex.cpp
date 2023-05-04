#include "stdafx.h"
#include "HentsIndex.h"

void HentsIndex::add(Hent* hent)
{
	m_index.insert(hent);
}

Hent* HentsIndex::find(uint32_t hentId)
{
	boost::multi_index::index<IndexType, hentsindex_tag_hentId>::type& hentsById = boost::multi_index::get<hentsindex_tag_hentId>(m_index);
	auto hentIt = hentsById.find(hentId);
	if (hentIt != hentsById.end())
		return *hentIt;

	return NULL;
}

Hent* HentsIndex::find(const HentNo& hentNo)
{
	boost::multi_index::index<IndexType, hentsindex_tag_hentNo>::type& hentsByHentNo = boost::multi_index::get<hentsindex_tag_hentNo>(m_index);
	auto hentIt = hentsByHentNo.find(hentNo);
	if (hentIt != hentsByHentNo.end())
		return *hentIt;

	return NULL;
}

void HentsIndex::remove(uint32_t hentId)
{
	boost::multi_index::index<IndexType, hentsindex_tag_hentId>::type& hentsById = boost::multi_index::get<hentsindex_tag_hentId>(m_index);
	hentsById.erase(hentId);
}

void HentsIndex::clear()
{
	m_index.clear();
}