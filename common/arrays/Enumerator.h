#ifndef __ENUMERATOR_H__
#define __ENUMERATOR_H__


#include <functional>
#include <boost/range/concepts.hpp>
#include <boost/range/detail/any_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/type_traits.hpp>

template<class from_type, class to_type1>
class cast_ptr : public std::unary_function<from_type,to_type1>
{
public:
	typedef std::unary_function<from_type, to_type1> _Base;
	typedef typename _Base::argument_type argument_type;
	typedef typename _Base::result_type result_type;

	
	result_type operator()(argument_type _Right) const
	{
		return (result_type)_Right;
	}

};


namespace myboost
{

	template<
		class Value
		, class Traversal
		, class Reference
		, class Difference
		, class Buffer
	>
		class any_iterator
		: public boost::iterators::iterator_facade<
		any_iterator<
		Value
		, Traversal
		, Reference
		, Difference
		, Buffer
		>
		, Value
		, Traversal
		, Reference
		, Difference
		>
	{
		template<
			class OtherValue
			, class OtherTraversal
			, class OtherReference
			, class OtherDifference
			, class OtherBuffer
		>
			friend class any_iterator;

		struct enabler {};
		struct disabler {};

		typedef typename boost::range_detail::any_iterator_interface_type_generator<
			Traversal
			, Reference
			, Difference
			, Buffer
		>::type abstract_base_type;

		typedef boost::iterators::iterator_facade<
			any_iterator<
			Value
			, Traversal
			, Reference
			, Difference
			, Buffer
			>
			, Value
			, Traversal
			, Reference
			, Difference
		> base_type;

		typedef Buffer buffer_type;

	public:
		typedef typename base_type::value_type value_type;
		typedef typename base_type::reference reference;
		typedef typename base_type::difference_type difference_type;

		// Default constructor
		any_iterator()
			: m_impl(0) {}

		// Simple copy construction without conversion
		any_iterator(const any_iterator& other)
			: base_type(other)
			, m_impl(other.m_impl
				? other.m_impl->clone(m_buffer)
				: 0)
		{
		}

		// Simple assignment operator without conversion
		any_iterator& operator=(const any_iterator& other)
		{
			if (this != &other)
			{
				if (m_impl)
					m_impl->~abstract_base_type();
				m_buffer.deallocate();
				m_impl = 0;
				if (other.m_impl)
					m_impl = other.m_impl->clone(m_buffer);
			}
			return *this;
		}

		// Implicit conversion from another any_iterator where the
		// conversion is from a non-const reference to a const reference
		template<
			class OtherValue
			, class OtherTraversal
			, class OtherReference
			, class OtherDifference
		>
			any_iterator(const any_iterator<
				OtherValue,
				OtherTraversal,
				OtherReference,
				OtherDifference,
				Buffer
			>& other,
				typename ::boost::enable_if<
				typename boost::mpl::and_<
				typename boost::range_detail::is_mutable_reference<OtherReference>::type,
				typename boost::range_detail::is_const_reference<Reference>::type
				>::type,
				enabler
				>::type* = 0
			)
			: m_impl(other.m_impl
				? other.m_impl->clone_const_ref(m_buffer)
				: 0
			)
		{
		}

		// Implicit conversion from another any_iterator where the
		// reference types of the source and the target are references
		// that are either both const, or both non-const.
		template<
			class OtherValue
			, class OtherTraversal
			, class OtherReference
			, class OtherDifference
		>
			any_iterator(const any_iterator<
				OtherValue
				, OtherTraversal
				, OtherReference
				, OtherDifference
				, Buffer
			>& other,
				typename ::boost::enable_if<
				typename boost::mpl::or_<
				typename boost::mpl::and_<
				typename boost::range_detail::is_mutable_reference<OtherReference>::type,
				typename boost::range_detail::is_mutable_reference<Reference>::type
				>::type,
				typename boost::mpl::and_<
				typename boost::range_detail::is_const_reference<OtherReference>::type,
				typename boost::range_detail::is_const_reference<Reference>::type
				>::type
				>::type,
				enabler
				>::type* = 0
			)
			: m_impl(other.m_impl
				? other.m_impl->clone(m_buffer)
				: 0
			)
		{
		}

		// Implicit conversion to an any_iterator that uses a value for
		// the reference type.
		template<
			class OtherValue
			, class OtherTraversal
			, class OtherReference
			, class OtherDifference
		>
			any_iterator(const any_iterator<
				OtherValue
				, OtherTraversal
				, OtherReference
				, OtherDifference
				, Buffer
			>& other,
				typename ::boost::enable_if<
				typename boost::range_detail::is_convertible_to_value_as_reference<
				OtherReference
				, Reference
				>::type,
				enabler
				>::type* = 0
			)
			: m_impl(other.m_impl
				? other.m_impl->clone_reference_as_value(m_buffer)
				: 0
			)
		{
		}

		any_iterator clone() const
		{
			any_iterator result;
			if (m_impl)
				result.m_impl = m_impl->clone(result.m_buffer);
			return result;
		}

		any_iterator<
			Value
			, Traversal
			, typename abstract_base_type::const_reference
			, Difference
			, Buffer
		>
			clone_const_ref() const
		{
			typedef any_iterator<
				Value
				, Traversal
				, typename abstract_base_type::const_reference
				, Difference
				, Buffer
			> result_type;

			result_type result;

			if (m_impl)
				result.m_impl = m_impl->clone_const_ref(result.m_buffer);

			return result;
		}

		// implicit conversion and construction from type-erasure-compatible
		// iterators
		template<class WrappedIterator>
		explicit any_iterator(
			const WrappedIterator& wrapped_iterator,
			typename boost::disable_if<	
			typename boost::mpl::or_<
					typename boost::range_detail::is_any_iterator<WrappedIterator>::type,
			typename boost::mpl::not_<
			typename boost::is_base_of< typename boost::remove_pointer<Value>::type, typename boost::remove_pointer<typename boost::iterators::iterator_value<WrappedIterator>::type >::type	>::type >::type 

					
			
			
			       >
			, disabler>::type* = 0
		)
		{	
		
			typedef typename boost::range_detail::any_iterator_wrapper_type_generator <
				WrappedIterator
				, Traversal
				, typename boost::iterators::iterator_reference<WrappedIterator>::type
				, Difference
				, Buffer
			>::type wrapper_type;
			
			void* ptr = m_buffer.allocate(sizeof(wrapper_type));
			m_impl = (abstract_base_type*) new(ptr) wrapper_type(wrapped_iterator);
		}

		~any_iterator()
		{
			// manually run the destructor, the deallocation is automatically
			// handled by the any_iterator_small_buffer base class.
			if (m_impl)
				m_impl->~abstract_base_type();
		}

	private:
		friend class ::boost::iterator_core_access;

		Reference dereference() const
		{
			BOOST_ASSERT(m_impl);
			return  m_impl->dereference();
		}

		bool equal(const any_iterator& other) const
		{
			return (m_impl == other.m_impl)
				|| (m_impl && other.m_impl && m_impl->equal(*other.m_impl));
		}

		void increment()
		{
			BOOST_ASSERT(m_impl);
			m_impl->increment();
		}

		void decrement()
		{
			BOOST_ASSERT(m_impl);
			m_impl->decrement();
		}

		Difference distance_to(const any_iterator& other) const
		{
			return m_impl && other.m_impl
				? m_impl->distance_to(*other.m_impl)
				: 0;
		}

		void advance(Difference offset)
		{
			BOOST_ASSERT(m_impl);
			m_impl->advance(offset);
		}

		any_iterator& swap(any_iterator& other)
		{
			BOOST_ASSERT(this != &other);
			// grab a temporary copy of the other iterator
			any_iterator tmp(other);

			// deallocate the other iterator, taking care to obey the
			// class-invariants in-case of exceptions later
			if (other.m_impl)
			{
				other.m_impl->~abstract_base_type();
				other.m_buffer.deallocate();
				other.m_impl = 0;
			}

			// If this is a non-null iterator then we need to put
			// a clone of this iterators implementation into the other
			// iterator.
			// We can't just swap because of the small buffer optimization.
			if (m_impl)
			{
				other.m_impl = m_impl->clone(other.m_buffer);
				m_impl->~abstract_base_type();
				m_buffer.deallocate();
				m_impl = 0;
			}

			// assign to this instance a clone of the temporarily held
			// tmp which represents the input other parameter at the
			// start of execution of this function.
			if (tmp.m_impl)
				m_impl = tmp.m_impl->clone(m_buffer);

			return *this;
		}

		buffer_type m_buffer;
		abstract_base_type* m_impl;
	};
}

template<typename PtrType>
class PtrEnumerator
{
	

	typedef myboost::any_iterator<PtrType*, boost::forward_traversal_tag, PtrType*, std::ptrdiff_t , boost::any_iterator_default_buffer > enum_iterator;

	enum_iterator m_endIt;
	enum_iterator m_It;

public:

	PtrEnumerator(const PtrEnumerator<PtrType>& src) : m_endIt(src.m_endIt), m_It(src.m_It)
	{
	}

	template<class T>
	PtrEnumerator(const T & collection)
	{
		typedef typename T::value_type value_type;

		auto it = boost::begin(collection);
		
		m_It = enum_iterator(it);

		auto end_it = boost::end(collection);
	
		m_endIt = enum_iterator(end_it);
	}

	template<class IT>
	PtrEnumerator(IT beginIt, IT endIt) 
	{
		typedef typename boost::iterator_value<IT>::type value_type;

		auto transform_it = boost::make_transform_iterator< cast_ptr<value_type, PtrType*> >(beginIt);
		m_It = enum_iterator(transform_it);

		
		auto end_transform_it = boost::make_transform_iterator< cast_ptr<value_type, PtrType*> >(endIt);
		m_endIt = enum_iterator(end_transform_it);
	}

	void advance()
	{
		++m_It;
	}

	bool operator==(const PtrEnumerator<PtrType>& rhs)
	{
		return m_It == rhs.m_It;
	}

	bool operator!=(const PtrEnumerator<PtrType>& rhs)
	{
		return m_It != rhs.m_It;
	}

	PtrType* operator*()
	{
		return *m_It;
	}

	bool hasNext()
	{
		return m_It != m_endIt;
	};

	PtrType* operator->()
	{
		return *m_It;
	}
};

template<class PtrType,class T>
PtrEnumerator<PtrType> make_enumerator(T & collection)
{

}

#endif
