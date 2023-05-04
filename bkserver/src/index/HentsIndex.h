#pragma once

#include "../object/HentServerObject.h"
#include "IdableSOIndex.h"

struct hentsindex_tag_hentId{};
struct hentsindex_tag_hentNo{};

struct HentIdKeyExtractor : public std::unary_function<Hent*, uint32_t>
{  
	uint32_t operator()(Hent* hent) const
	{
		return hent->GetId();
	}
};

struct HentNoKeyExtractor : public std::unary_function<Hent*, HentNo>
{
  HentNo operator()(Hent* hent) const
  {
		return hent->GetHentNo();
  }
};


class HentsIndex
{
	typedef boost::multi_index_container
	<
	Hent*,
	boost::multi_index::indexed_by<
	boost::multi_index::ordered_unique < boost::multi_index::tag<hentsindex_tag_hentId>, HentIdKeyExtractor >,
	boost::multi_index::ordered_unique < boost::multi_index::tag<hentsindex_tag_hentNo>, HentNoKeyExtractor >
	>
	> IndexType;
public:
	void add(Hent* hent);
	Hent* find(uint32_t hentId);
	Hent* find(const HentNo& hentNo);
	void remove(uint32_t hentId);
	void clear();
private:
	IndexType m_index;
};

typedef SOKeyExtrator<HentServerObject, HentIdKeyExtractor> HentServerObjectKeyHentId;
typedef SOKeyExtrator<HentServerObject, HentNoKeyExtractor> HentServerObjectKeyHentNo;

typedef boost::multi_index_container
<
	HentServerObject*,
	boost::multi_index::indexed_by< 
									boost::multi_index::ordered_unique < boost::multi_index::tag<hentsindex_tag_hentId>, HentServerObjectKeyHentId >,
									boost::multi_index::ordered_unique < boost::multi_index::tag<hentsindex_tag_hentNo>, HentServerObjectKeyHentNo >
								  >
> HentsSOIndex;

