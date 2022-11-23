// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// gdi/point.hpp:
//      - Specialization of box_traits for POINT

#ifndef BOOST_GEOM_GDI_POINT_HPP
#define BOOST_GEOM_GDI_POINT_HPP

// Boost.Geom
#include <boost/geom/point_traits.hpp>
#include <boost/geom/point_xy_traits.hpp>
#include <boost/geom/detail/to_float_type.hpp>

// Windows API
#include <windows.h> // for POINT (maybe a bit expensive ...)

namespace boost { namespace geom {

namespace detail {

struct gdi_point_prop
{
	typedef LONG  Value;
	typedef float Float;

    private:
	typedef POINT point_type;

    public:
	struct x 
	{
		static const bool direct_access=true;
		typedef Value      value_type;
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
		typedef Value      value_type;
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
// back in namespace boost::geom

/// Specializing point_traits to deal with POINT
///  See geom/point_xy_traits.hpp
template<>
struct point_traits< POINT >:
	public point_xy_traits<POINT,
			       typename detail::gdi_point_prop::x,
			       typename detail::gdi_point_prop::y,
			       float>
				
{
	static const bool is_specialized=true;
};

} } // namespace boost::geom

#endif // ifndef BOOST_GEOM_GDI_POINT_HPP
