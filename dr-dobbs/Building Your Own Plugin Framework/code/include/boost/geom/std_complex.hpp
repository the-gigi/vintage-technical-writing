// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// std_complex.hpp:
//	- Specialization of point_traits<> for class std::complex<>

#ifndef BOOST_GEOM_STD_COMPLEX_HPP
#define BOOST_GEOM_STD_COMPLEX_HPP

// Standard C++
#include <complex>

// Boost.Geom
#include <boost/geom/detail/to_float_type.hpp>
#include <boost/geom/point_traits.hpp>
#include <boost/geom/detail/dummy_property.hpp>

namespace boost { namespace geom {

template<typename Value>
struct point_traits< std::complex<Value> >
{
	typedef std::complex<Value> point_type;
	static const bool is_specialized=true;

	struct x 
	{
		static const bool direct_access=false;
		
		typedef Value      value_type;
		typedef value_type ret_type;
		
		static void set(point_type& self, value_type v)
		{
			self=point_type(v, self.imag());
		}

		static ret_type get(const point_type& self)
		{ return self.real(); }

		static value_type& getset(point_type&);
	};

	struct y
	{
		static const bool direct_access=false;
		
		typedef Value      value_type;
		typedef value_type ret_type;
		
		static void set(point_type& self, value_type v)
		{
			self=point_type(self.real(), v);
		}

		static ret_type get(const point_type& self)
		{ return self.imag(); }

		static value_type& getset(point_type&);
	};

	typedef detail::fixed_property<point_type, Value, 0> z;

	
	struct rho
	{
		static const bool direct_access=false;
		
		typedef Value      value_type;
		typedef value_type ret_type;

		static void set(point_type& h, value_type v)
		{
			value_type factor=v/rho::get(h);
			x::set(h, static_cast<typename x::value_type>(x::get(h)*factor));		
			y::set(h, static_cast<typename y::value_type>(y::get(h)*factor));
		}

		static ret_type get(const point_type& h)
		{ return std::abs(h);}

		static value_type& getset(point_type&); // Never defined
	};
	
	struct theta
	{
		static const bool direct_access=false;
		
		typedef Value      value_type;
		typedef value_type ret_type;

		static void set(point_type& self, value_type v)
		{
			value_type r=rho::get(self);
			x::set(self, static_cast<typename x::value_type>(r*std::cos(v)));
			y::set(self, static_cast<typename y::value_type>(r*std::sin(v)));
		}

		static ret_type get(const point_type& h)
		{ return std::arg(h); }

		static value_type& getset(point_type&); // Never defined
	};

	typedef detail::fixed_property<point_type, Value, 0> phi;

	template<class Point>
	static void add(point_type& h, const Point& p)
	{
		x::set(h, x::get(h)+point_traits<Point>::x::get(p));
		y::set(h, y::get(h)+point_traits<Point>::y::get(p));
	}

	static void add(point_type& h, const std::complex<Value>& p)
	{h+=p;}

	template<class Point>
	static void sub(point_type& h, const Point& p)
	{
		x::set(h, x::get(h)-point_traits<Point>::x::get(p));
		y::set(h, y::get(h)-point_traits<Point>::y::get(p));
	}

	static void sub(point_type& h, const std::complex<Value>& p)
	{h-=p;}
	
	template<class BuiltIn>
	static void mul(point_type& h, BuiltIn p)
	{
		x::set(h, x::get(h)*p);
		y::set(h, y::get(h)*p);
	}

	static void mul(point_type& h, Value p)
	{h*=p;}


	template<class BuiltIn>
	static void div(point_type& h, BuiltIn p)
	{
		x::set(h, x::get(h)/p);
		y::set(h, y::get(h)/p);
	}

	static void div(point_type& h, Value p)
	{ h/=p; }

	static void neg(point_type& h)
	{
		x::set(h, -x::get(h));
		y::set(h, -y::get(h));
	}

	template<class Point>
	static bool equal(const point_type& h, const Point& p)
	{
		return  x::get(h)==point_traits<Point>::x::get(p) &&
		    y::get(h)==point_traits<Point>::y::get(p) &&
		    point_traits<Point>::z::get(p)==0;
	}

	static bool equal(const point_type& h, const point_type& p)
	{ return h==p; }


	template<class Point>
	static void assign(point_type& self, const Point& p)
	{
		x::set(self, point_traits<Point>::x::get(p));
		y::set(self, point_traits<Point>::y::get(p));
	}

	static void assign(point_type& self, const point_type& p)
	{self=p;}

};

} } // namespace boost::geom
					

#endif // #ifndef BOOST_GEOM_STD_COMPLEX_HPP
