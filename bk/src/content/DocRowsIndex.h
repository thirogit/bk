#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>


template<class ROW>
struct DocRowsIndex
{
	struct tag_Id{};

	struct RowKeyExtractor
	{
		typedef uint32_t result_type;
		result_type operator()(ROW* pRow) const 
		{
			return pRow->Content()->GetDocId();
		}
	};

	typedef boost::multi_index_container
	<
		ROW*,
		boost::multi_index::indexed_by< 
										boost::multi_index::random_access<>,
										boost::multi_index::ordered_unique < boost::multi_index::tag<tag_Id>, RowKeyExtractor >
										>
	> IndexType;


	typedef ROW* value_type;

	ROW* remove_row(uint32_t docId)
	{
		boost::multi_index::index<IndexType,tag_Id>::type&  index = m_index.get<tag_Id>();
		auto rowIt = index.find(docId);	
		if(rowIt != index.end())
		{
			ROW* pRow = *rowIt;
			index.erase(rowIt);	
			return pRow;		
		}

		return NULL;
	}

	ROW* find_row(uint32_t id)
	{
		boost::multi_index::index<IndexType,tag_Id>::type&  index = m_index.get<tag_Id>();
		auto rowIt = index.find(id);	
		if(rowIt != index.end())
		{
			return *rowIt;
		}
		return NULL;
	}

	void add_row(ROW* row)
	{
		m_index.push_back(row);
	}

	void clear()
	{
		m_index.clear();
	}

	size_t size() const
	{
		return m_index.size();
	}

	typename IndexType::iterator begin()
	{
		return m_index.begin();
	}

	typename IndexType::iterator end()
	{
		return m_index.end();
	}

	ROW* row_at(int index)
	{			
		return m_index[index];		
	}

	template<typename CMPTR>
	void sort(CMPTR& comparator)
	{
		m_index.sort(comparator);
	}



private:
	IndexType m_index;

};

	
