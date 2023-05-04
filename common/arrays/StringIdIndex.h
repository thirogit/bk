#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

struct StringIdIndex_Tag_Id{};


template<class StringIdType>
struct StringIdKeyExtractor
{
	typedef std::wstring result_type;
	result_type operator()(StringIdType* idable) const
	{
		return idable->GetId();
	}
};


template<class StringIdType>
struct StirngIdIndex 
{
	typedef boost::multi_index_container
	<
	StringIdType*,
	boost::multi_index::indexed_by< 
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<StringIdIndex_Tag_Id>, StringIdKeyExtractor<StringIdType> >
		>
	> ContainerType;
	typedef typename boost::multi_index::index<ContainerType, StringIdIndex_Tag_Id>::type IndexByStringId;
public:

	typedef typename ContainerType::iterator iterator;
	typedef typename ContainerType::const_iterator const_iterator;
	typedef StringIdType* value_type;

	StringIdType* find(const std::wstring& id)
	{
		IndexByStringId& indexById = boost::multi_index::get<StringIdIndex_Tag_Id>(m_container);
		auto idableIt = indexById.find(id);
		if(idableIt != indexById.end())
			return *idableIt;
		return NULL;
	}

	size_t size() const
	{
		return m_container.size();
	}

	bool empty() const
	{
		return size() == 0;
	}

	StringIdType* at(int index)
	{
		return m_container.at(index);			
	}

	void remove(uint32_t id)
	{
		IndexByStringId& indexById = boost::multi_index::get<StringIdIndex_Tag_Id>(m_container);
		indexById.erase(id);
	}

	void add(StringIdType* element)
	{
		m_container.push_back(element);
	}

	iterator begin()
	{
		return m_container.begin();
	}

	iterator end()
	{
		return m_container.end();
	}

	const_iterator begin() const
	{
		return m_container.cbegin();
	}

	const_iterator end() const
	{
		return m_container.cend();
	}


	void clear()
	{
		m_container.clear();
	}
private:
	ContainerType m_container;
	
};

