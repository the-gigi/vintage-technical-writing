// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// point_xy_traits.hpp:
// 	- Traits generator point_xy_traits<>

#ifndef BOOST_GEOM_POINT_XY_TRAITS_HPP
#define BOOST_GEOM_POINT_XY_TRAITS_HPP

//--Standard C++----------------------------------------------------------------
#include <cmath> // for cos, sin, sqrt, atan2 ..

#include <boost/geom/detail/dummy_property.hpp>

namespace boost { namespace geom {

// Defines point properties based on a subset of them, namely X, Y ( Z
// is supposed to be always zero).
// This is intended to help specialize point_traits.
// Host: The point_type hosting the properties
// X: The X property of Host
// Y: The Y property of Host
// Float: Used to implement RHO, THETA and PHI ...
/// The value_type of the Z property will be typename X::value_type
template<class Host, class X, class Y, typename Float>
struct point_xy_traits
{
	typedef Host point_type;
	
	// X, Y and Z already are provided so ...
	typedef X x;
	typedef Y y;
	typedef detail::fixed_property<Host, typename X::value_type, 0> z;

	struct rho
	{
		static const bool direct_access=false;
		
		typedef Float    value_type;
		typedef value_type ret_type;

		static void set(Host& h, value_type v)
		{
			value_type factor=v/rho::get(h);
			x::set(h, static_cast<typename x::value_type>(x::get(h)*factor));		
			y::set(h, static_cast<typename y::value_type>(y::get(h)*factor));
		}

		static ret_type get(const Host& h)
		{
			value_type xx=X::get(h);
			value_type yy=Y::get(h);
			return std::sqrt(static_cast<value_type>(xx*xx+yy*yy));
		}

		static value_type& getset(Host&); // Never defined
	};

	struct theta
	{
		static const bool direct_access=false;
		
		typedef Float      value_type;
		typedef value_type ret_type;

		static void set(Host& self, value_type v)
		{
			value_type r=rho::get(self);
			x::set(self, static_cast<typename x::value_type>(r*std::cos(v)));
			y::set(self, static_cast<typename y::value_type>(r*std::sin(v)));
		}

		static ret_type get(const Host& h)
		{
			return std::atan2(static_cast<value_type>(y::get(h)),
					  static_cast<value_type>(x::get(h)));
		}
		
		static value_type& getset(Host&); // Never defined
	};
	
	typedef detail::fixed_property<Host, typename X::value_type, 0> phi;
	
	// Ops
	template<class Point>
	static void add(Host& h, const Point& p)
	{
		x::set(h, x::get(h)+point_traits<Point>::x::get(p));
		y::set(h, y::get(h)+point_traits<Point>::y::get(p));
	}

	template<class Point>
	static void sub(Host& h, const Point& p)
	{
		x::set(h, x::get(h)-point_traits<Point>::x::get(p));
		y::set(h, y::get(h)-point_traits<Point>::y::get(p));
	}

	template<class BuiltIn>
	static void mul(Host& h, BuiltIn p)
	{
		x::set(h, x::get(h)*p);
		y::set(h, y::get(h)*p);
	}

	template<class BuiltIn>
	static void div(Host& h, BuiltIn p)
	{
		x::set(h, x::get(h)/p);
		y::set(h, y::get(h)/p);
	}

	static void neg(Host& h)
	{
		x::set(h, -x::get(h));
		y::set(h, -y::get(h));
	}

	template<class Point>
	static bool equal(const Host& h, const Point& p)
	{
		return  x::get(h)==point_traits<Point>::x::get(p) &&
		    y::get(h)==point_traits<Point>::y::get(p) &&
		    point_traits<Point>::z::get(p)==0;
	}

	template<class Point>
	static void assign(Host& self, const Point& p)
	{
		x::set(self, point_traits<Point>::x::get(p));
		y::set(self, point_traits<Point>::y::get(p));
	}
	
};// struct point_xy_traits


} } // namespace boost::geom

#endif // #ifndef BOOST_GEOM_POINT_XY_TRAITS_HPP
