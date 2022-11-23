// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// x/point.hpp:
//	- Specialization of point_traits<> for XPoint

#ifndef BOOST_GEOM_X_POINT_HPP
#define BOOST_GEOM_X_POINT_HPP

// Xlib
#include <X11/Xlib.h>

// Boost.Geom
#include <boost/geom/point_traits.hpp>
#include <boost/geom/point_xy_traits.hpp>
#include <boost/geom/detail/to_float_type.hpp>

namespace boost { namespace geom {

namespace detail {
struct x_point_prop
{
    private:
	typedef XPoint point_type;

    public:
	struct x 
	{
		static const bool direct_access=true;
		typedef short      value_type;
		typedef value_type ret_type;
		
		static ret_type get(const point_type& self)
		{ return self.x; }

		static void set(point_type& self, value_type v)
		{ self.x=v; }

		static value_type& getset(point_type& self)
		{ return self.x; }
	};

	struct y
	{
		static const bool direct_access=true;
		typedef short      value_type;
		typedef value_type ret_type;
		
		static ret_type get(const point_type& self)
		{ return self.y; }

		static void set(point_type& self, value_type v)
		{ self.y=v; }

		static value_type& getset(point_type& self)
		{ return self.y; }
	};
};

		
} // namespace detail
// back in namespace boost { namespace geom

/// Specializing point_traits to deal with XPoint
///  See geom/point_xy_traits.hpp
template<>
struct point_traits< XPoint >:
	public point_xy_traits<XPoint,
			       detail::x_point_prop::x,
			       detail::x_point_prop::y,
			       double>
				
{
	static const bool is_specialized=true;
};

} } // namespace boost::geom

#endif // ifndef BOOST_GEOM_X_POINT_HPP
