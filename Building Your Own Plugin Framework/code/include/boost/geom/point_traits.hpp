// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// point_traits.hpp:
//	- Point traits class (geom::point_traits<>)
//	- Point implemenation extractor (geom::point_impl<>)

#ifndef BOOST_GEOM_POINT_TRAITS_HPP
#define BOOST_GEOM_POINT_TRAITS_HPP

#include <boost/geom/detail/dummy_property.hpp> // for dummy_property

namespace boost { namespace geom {

/// Traits class point_traits gives information on a particular
/// point-like type.
///
/// For example if you want to treat `long' as a point
/// simply specialize point_traits<long> with the desired
/// information/operations inside. Other parts of geom will then be
/// able to treat long as a point.
///
/// This is a dummy (uninteresting) definition, only useful to show
/// the set of requirements. Meaningful specializations exist.(see,
/// for example, geom/point_xy.hpp geom/point_xyz.hpp)
template<typename T>
struct point_traits
{
	typedef T point_type;

	static const bool is_specialized=false;

	// Properties (x, y, z, rho (magnitude), theta (phase angle),
	// phi (second phase angle) ..)

	/// See geom/detail/dummy_property.hpp for a defintion of detail::dummy_property
	typedef detail::dummy_property<T, int> x;
	typedef detail::dummy_property<T, int> y;
	typedef detail::dummy_property<T, int> z;
	
	typedef detail::dummy_property<T, double> rho;
	typedef detail::dummy_property<T, double> theta;
	typedef detail::dummy_property<T, double> phi;

	// Operations
	template<class Point>
	static void add(T& self, const Point&);

	template<class Point>
	static void sub(T& self, const Point&);

	template<class Point>
	static bool equal(const T& self, const Point&);

	template<class Point>
	static void assign(T&, const Point&);
	
	static void neg(T& self);
	
	template<class BuiltIn>
	static void mul(T& self, const BuiltIn&);

	template<class BuiltIn>
	static void div(T& self, const BuiltIn&);	
};

// Extracts implentation type and instance from a type.
//
// For types like point_xy, (which are themselves implementations)
// this does nothing but returning the arguments you passed.
//
// Class point partially specialize this to make sur its implentation
// informations can be reched using point_impl

template<class  T>
struct point_impl
{
	typedef T type;
	static type& extract(T& t)
	{ return t; }
	static const type& extract(const T& t)
	{ return t; }
};

} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_POINT_TRAITS_HPP
