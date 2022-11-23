// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

#ifndef BOOST_GEOM_POINT_XYZ_TRAITS_HPP
#define BOOST_GEOM_POINT_XYZ_TRAITS_HPP

//--Standard C++----------------------------------------------------------------

#include <cmath> // for cos, sin, sqrt, atan2 ..

#include <boost/type_traits/is_same.hpp>

#include <boost/geom/detail/dummy_property.hpp>
#include <boost/geom/point_xy_traits.hpp>

namespace boost { namespace geom {

// Defines point properties based on a subset of them, namely X, Y,
// and Z. This is intended to help specialize point_traits.
// Host: The point_type hosting the properties
// X: The X property of Host
// Y: The Y property of Host
// Z: The Z property of Host
// Float: Used to implement RHO, THETA and PHI ...

template<class Host, class X, class Y, class Z, typename Float>
struct point_xyz_traits
{
    private:
	typedef point_xy_traits<Host, X, Y, Float> base_type ;

    public:
	typedef Host point_type;
	
	// X, Y and Z already provided
	typedef X x;
	typedef Y y;
	typedef Z z;
	

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
			z::set(h, static_cast<typename z::value_type>(z::get(h)*factor));
		}

		static value_type get(const Host& h)
		{
			value_type xx=x::get(h);
			value_type yy=y::get(h);
			value_type zz=z::get(h);
			
			return std::sqrt(xx*xx+ yy*yy + zz*zz);
		}

		static value_type& getset(Host&); // Never defined
	};

	struct theta
	{
		static const bool direct_access=false;
		
		typedef Float      value_type;
		typedef value_type ret_type;

		static void set(Host& h, value_type v)
		{
			value_type r=rho::get(h);
			x::set(h, static_cast<typename x::value_type>(r*std::cos(v)));
			y::set(h, static_cast<typename y::value_type>(r*std::sin(v)));
		}

		static value_type get(const Host& h)
		{ return std::atan2(static_cast<value_type>(y::get(h)),
				    static_cast<value_type>(x::get(h))); }
	};
	
	struct phi 
	{
		static const bool direct_access=false;
		
		typedef Float      value_type;
		typedef value_type ret_type;
		
		static void set(Host& h, value_type v)
		{
			value_type rh=rho::get(h);
			value_type th=theta::get(h);
			
			x::set(h, static_cast<typename x::value_type>(rh*std::cos(v)*std::cos(th)));
			y::set(h, static_cast<typename y::value_type>(rh*std::cos(v)*std::sin(th)));
		}
		
		static value_type get(const Host& h)
		{
			// asin (z/rho)
			return std::asin(static_cast<value_type>(z::get(h))/rho::get(h));
		}

		static value_type& getset(Host&); // Never defined
	};

	// Ops
	template<class Point>
	static void add(Host& h, const Point& p)
	{
		x::set(h, x::get(h)+point_traits<Point>::x::get(p));
		y::set(h, y::get(h)+point_traits<Point>::y::get(p));
		z::set(h, z::get(h)+point_traits<Point>::z::get(p));	
	}

	template<class Point>
	static void sub(Host& h, const Point& p)
	{
		x::set(h, x::get(h)-point_traits<Point>::x::get(p));
		y::set(h, y::get(h)-point_traits<Point>::y::get(p));
		z::set(h, z::get(h)-point_traits<Point>::z::get(p));	
	}

	template<class BuiltIn>
	static void mul(Host& h, BuiltIn p)
	{
		x::set(h, x::get(h)*p);
		y::set(h, y::get(h)*p);
		z::set(h, z::get(h)*p);
	}

	template<class BuiltIn>
	static void div(Host& h, BuiltIn p)
	{
		x::set(h, x::get(h)/p);
		y::set(h, y::get(h)/p);
		z::set(h, z::get(h)/p);
	}

	static void neg(Host& h)
	{
		x::set(h, -x::get(h));
		y::set(h, -y::get(h));
		z::set(h, -z::get(h));
	}

	template<class Point>
	static bool equal(const Host& h, const Point& p)
	{
		return  x::get(h)==point_traits<Point>::x::get(p) &&
		    y::get(h)==point_traits<Point>::y::get(p) &&
		    z::get(h)==point_traits<Point>::z::get(p);	 
	}

	template<class Point>
	static void assign(Host& self, const Point& p)
	{
		x::set(self, point_traits<Point>::x::get(p));
		y::set(self, point_traits<Point>::y::get(p));
		z::set(self, point_traits<Point>::z::get(p));
	}
	
};// struct point_xyz_traits


} } // namespace boost::geom

#endif // #ifndef BOOST_GEOM_POINT_XYZ_TRAITS_HPP
