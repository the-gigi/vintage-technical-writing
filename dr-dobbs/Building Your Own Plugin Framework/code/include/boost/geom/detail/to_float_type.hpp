// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// detail/to_float_type.hpp:
// 	- Class to_float_type<>

#ifndef BOOST_GEOM_DETAIL_TO_FLOAT_TYPE_HPP
#define BOOST_GEOM_DETAIL_TO_FLOAT_TYPE_HPP

//--Boost-----------------------------------------------------------------------
#include <boost/mpl/if.hpp> // for boost::mpl::if_c
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace geom { namespace detail {

/// Template meta-function that returns the float type
/// ``corresponding'' to Value 
template <typename Value>
struct to_float_type 
{
 	// This (fairly complicated stuff) returns Value if Value is
	// float, double or long double. Otherwise, this typdefs
	// float_type to double.
	typedef typename boost::mpl::if_c<boost::is_same<Value, double>::value ||
		boost::is_same<Value, float>::value ||
	boost::is_same<Value, long double>::value, Value, double>::type type;
};
 
} } } // namespace boost::geom::detail


#endif // #ifndef BOOST_GEOM_DETAIL_TO_FLOAT_TYPE_HPP
