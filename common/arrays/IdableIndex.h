#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <data/IIdable.h>
#include <boost/multi_index/random_access_index.hpp>

struct IdableIndex_Tag_Id{};


template<class IdableType>
struct IdableKeyExtractor
{
	typedef uint32_t result_type;
	result_type operator()(IdableType* idable) const
	{
		return idable->GetId();
	}
};


template<class IdableType>
struct IdableIndex 
{
	typedef boost::multi_index_container
	<
	IdableType*,
	boost::multi_index::indexed_by< 
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<IdableIndex_Tag_Id>, IdableKeyExtractor<IdableType> >  
		>
	> ContainerType;
	typedef typename boost::multi_index::index<ContainerType, IdableIndex_Tag_Id>::type IndexById;
public:

	typedef typename ContainerType::iterator iterator;
	typedef typename ContainerType::const_iterator const_iterator;
	typedef IdableType* value_type;

	IdableType* find(uint32_t id)
	{
		IndexById& indexById = boost::multi_index::get<IdableIndex_Tag_Id>(m_container);
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

	IdableType* at(int index)
	{
		return m_container.at(index);			
	}

	void remove(uint32_t id)
	{
		IndexById& indexById = boost::multi_index::get<IdableIndex_Tag_Id>(m_container);
		indexById.erase(id);
	}

	void add(IdableType* element)
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

