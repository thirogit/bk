#pragma once

#include <boost/type_traits.hpp>
#include <algorithm>
#include <map>
#include <unordered_map>

template <typename Container> struct is_map : std::false_type { };

template <class _Kty,class _Ty,class _Pr,class _Alloc> struct is_map<std::map<_Kty,_Ty,_Pr,_Alloc> > : std::true_type { };
template <class _Kty,class _Ty,class _Pr,class _Alloc> struct is_map<std::unordered_map<_Kty,_Ty,_Pr,_Alloc> > : std::true_type { };
template <class _Kty, class _Ty, class _Pr, class _Alloc> struct is_map<std::multimap<_Kty, _Ty, _Pr, _Alloc> > : std::true_type{};

template<class CONTAINER>
	typename boost::disable_if<is_map<CONTAINER>>::type DeepDelete(CONTAINER& container)
	{
		std::for_each( container.begin(), container.end(), [](typename CONTAINER::value_type element)
			{
				delete element;
			});
	}


template<class CONTAINER>
	typename boost::enable_if<is_map<CONTAINER>>::type DeepDelete(CONTAINER& container)
	{
		std::for_each( container.begin(), container.end(), [](typename CONTAINER::value_type element)
			{
				delete element.second;
			});
	}