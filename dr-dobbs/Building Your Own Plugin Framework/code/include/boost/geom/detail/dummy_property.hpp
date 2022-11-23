// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// detail/builtin_wrapper.hpp:
// 	- Class fixed_property<>
//	- Class dummy_property<>

#ifndef BOOST_GEOM_DETAIL_DUMMY_PROPERTY
#define BOOST_GEOM_DETAIL_DUMMY_PROPERTY

namespace boost { namespace geom { namespace detail {

/// Used in traits classes when some property is not really held (as a
/// data field) inside the implementation. This will make the property
/// fixed to "val" (i.e the property is supposed a built-in type).
//// Host: is supposed the type owning the property (like point or box)
//// Property: is supposed to be the property type (like int or double)
template<typename Host, typename Property, int value>
struct fixed_property
{
	typedef Property   value_type;
	typedef value_type ret_type;

	static const bool direct_access=false;
	
	static void set(Host& h, value_type)
	{}; // Nop !!!

	static value_type get(const Host&)
	{ return static_cast<value_type>(value); }// ``value'' no matter !

	static value_type& getset(Host&); // Never defined
};


/// Used in the general definition of traits classes
//// Host: is supposed the type owning the property (like point or box)
//// Property: is supposed to be the property type (like int or double)
template<typename Host, typename Property>
struct dummy_property
{
	typedef Property   value_type;
	typedef value_type ret_type;

	static const bool direct_access=false;
	
	static void set(Host& h, value_type); // Never defined !!

	static value_type get(const Host&); // Never defined !!
	
	static value_type& getset(Host&); // Never defined !!
};
 
} } } // namespace boost::geom::detail
	
#endif // #ifndef BOOST_GEOM_DETAIL_DUMMY_PROPERTY
