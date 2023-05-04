#pragma once

 #include <boost/type_traits.hpp>

template < typename R
         , typename CLASS
         , typename DEF >
		  typename boost::remove_const< typename boost::remove_reference< R >::type >::type SafeGet(R(CLASS::*f)() const,CLASS* obj, DEF def)
{
	if(obj != NULL)
		return (obj->*f)();
		
	return def;
}


template < typename R
         , typename CLASS
         , typename DEF >
		  typename boost::remove_const< typename boost::remove_reference< R >::type >::type SafeGet(R(CLASS::*f)() const,CLASS* obj1,CLASS* obj2, DEF def)
{
	if(obj1 != NULL)
		return (obj1->*f)();

	if(obj2 != NULL)
		return (obj2->*f)();
		
	return def;
}