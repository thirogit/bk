#ifndef __PTRRANGE_H__
#define __PTRRANGE_H__

#include <boost/range/concepts.hpp>
#include <boost/range/detail/any_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>

template<typename PtrType>
class PtrRange
{
	template<class from_type, class to_type>
	class cast_ptr : public std::unary_function<typename from_type, typename to_type>
	{
	public:
		typedef std::unary_function<typename from_type, typename to_type> _Base;
		typedef typename _Base::argument_type argument_type;
		typedef typename _Base::result_type result_type;

		result_type operator()(const argument_type& _Right) const
		{
			return (result_type)_Right;
		}

		result_type operator()(argument_type& _Right) const
		{
			return (result_type)_Right;
		}

	protected:

		typename from_type value;
	};
	
	typedef boost::any_range<PtrType*, boost::forward_traversal_tag, PtrType*, std::ptrdiff_t> range_type;

	
	range_type m_range;

public:

	typedef typename range_type::iterator iterator;
	typedef PtrRange* value_type;


	PtrRange(const PtrRange<PtrType>& src) : m_range(src.m_range)
	{
	}

	template<class T>
	PtrRange(T & collection)
	{
		typedef T::value_type value_type;

		auto it = boost::begin(collection);
		auto transform_it = boost::make_transform_iterator< cast_ptr<value_type, PtrType*> >(it);
		

		auto end_it = boost::end(collection);
		auto end_transform_it = boost::make_transform_iterator< cast_ptr<value_type, PtrType*> >(end_it);
		

		m_range = range_type(transform_it, end_transform_it);
	}


	template<class IT>
	PtrRange(IT beginIt, IT endIt) 
	{
		typedef boost::iterator_value<IT>::type value_type;

		auto transform_it = boost::make_transform_iterator< cast_ptr<value_type, PtrType*> >(beginIt);
		
		auto end_transform_it = boost::make_transform_iterator< cast_ptr<value_type, PtrType*> >(endIt);
				
		m_range = range_type(transform_it, end_transform_it);
	}

	iterator begin()
	{
		return m_range.begin();
	}

	iterator end()
	{
		return m_range.end();
	}

	

	
};




#endif
