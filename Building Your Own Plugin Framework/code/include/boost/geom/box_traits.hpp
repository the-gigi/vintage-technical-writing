// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// box_traits.hpp:
//	- Box traits class (geom::box_traits<>)
//	- Box implemenation extractor (geom::box_impl<>)

#ifndef BOOST_GEOM_BOX_TRAITS_HPP
#define BOOST_GEOM_BOX_TRAITS_HPP

#include <boost/geom/detail/dummy_property.hpp>

namespace boost { namespace geom {

/// Traits class box_traits gives information on a particular
/// box-like type.
///
/// For example if you want to treat `struct rect {long x,y,w,h;}' as a box
/// simply specialize box_traits<rect> with the desired
/// information/operations inside. Other parts of geom will then be
/// able to treat ``rect'' as a box.
///
/// This is a dummy (uninteresting) definition, only useful to show
/// the set of requirements. Meaningful specializations exist.(see,
/// for example, geom/box_xywh.hpp geom/box_xyzwhd.hpp)
template<typename T>
struct box_traits
{
	typedef T box_type;
	static const bool is_specialized=false;

	// Properties x1, x2, y1, y2, z1, z2, ...
	// ... xmax, xmin, ymax, ymin, zmin, zmax, width, ..
	// ... height, depth.

	/// See geom/detail/dummy_property.hpp
	typedef detail::dummy_property<T, int> x1;
	typedef detail::dummy_property<T, int> y1;
	typedef detail::dummy_property<T, int> z1;
	
	typedef detail::dummy_property<T, int> x2;
	typedef detail::dummy_property<T, int> y2;
	typedef detail::dummy_property<T, int> z2;

	////

	typedef detail::dummy_property<T, int> xmin;
	typedef detail::dummy_property<T, int> ymin;
	typedef detail::dummy_property<T, int> zmin;

	typedef detail::dummy_property<T, int> xmax;
	typedef detail::dummy_property<T, int> ymax;
	typedef detail::dummy_property<T, int> zmax;

	////
	
	typedef detail::dummy_property<T, int> width;
	typedef detail::dummy_property<T, int> height;
	typedef detail::dummy_property<T, int> depth;


	// Operations
	template<class Point>
	static void add(T& self, const Point&);

	template<class Point>
	static void sub(T& self, const Point&);

	template<class Box>
	static void or_op(T& self, const Box&);

	template<class Box>
	static void and_op(T& self, const Box&);
	
	template<class Box>
	static bool equal(const T& self, const Box&);

	template<class Box>
	static void assign(T& self, const Box&);
	
	static bool valid(const T& self);
};

template<class T>
struct box_impl
{
	typedef T type;
	
	static type& extract(T& t)
	{ return t; }

	static const type& extract(const T& t)
	{ return t; }
};

} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_BOX_TRAITS_HPP
