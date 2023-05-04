#pragma once

#include <functional>
#include "ServerObjectPeeler.h"
#include <arrays/IdableIndex.h>


template<class ServerObjectType, class KeyExtactor>
struct SOKeyExtrator : public std::unary_function<ServerObjectType, typename KeyExtactor::result_type>
{
	typename KeyExtactor::result_type operator()(ServerObjectType* pSO) const
	{
		return m_keyExtractor(m_Peeler(pSO));
	}

	ServerObjectPeeler<ServerObjectType> m_Peeler;
	KeyExtactor m_keyExtractor;
};

template<class SO>
class IdableSOIndex
{
public:

	typedef boost::multi_index_container
	<
	SO*,
	boost::multi_index::indexed_by< boost::multi_index::ordered_unique < boost::multi_index::tag<IdableIndex_Tag_Id>, SOKeyExtrator<SO, IdableKeyExtractor<typename SO::object_type>> >  >
	> ContainerType;

	typedef typename ContainerType::iterator iterator;
	typedef SO* value_type;
	typedef typename boost::multi_index::index<ContainerType, IdableIndex_Tag_Id>::type IndexById;

	void add(SO* so)
	{
		m_container.insert(so);
		typename ContainerType::iterator it = m_container.begin();
	}

	SO* find(uint32_t id)
	{
		IndexById& indexById = boost::multi_index::get<IdableIndex_Tag_Id>(m_container);
		auto it = indexById.find(id);
		if (it != indexById.end())
			return *it;

		return NULL;
	}

	void remove(uint32_t id)
	{
		IndexById& indexById = boost::multi_index::get<IdableIndex_Tag_Id>(m_container);
		indexById.erase(id);
	}



	iterator begin()
	{
		return m_container.begin();
	}

	iterator end()
	{
		return m_container.end();
	}

	size_t size() const
	{
		return m_container.size();
	}
	

private:

	ContainerType m_container;
};

