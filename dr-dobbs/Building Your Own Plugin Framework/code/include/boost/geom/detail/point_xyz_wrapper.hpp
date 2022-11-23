// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// detail/poinr_xyz_wrapper.hpp:
// 	- Implementation type point_xyz_wrapper<>
//	- Specialization of point_traits<> for point_xyz_wrapper<>

#ifndef BOOST_GEOM_DETAIL_POINT_XYZ_WRAPPER
#define BOOST_GEOM_DETAIL_POINT_XYZ_WRAPPER

#include <boost/geom/point_traits.hpp>
#include <boost/geom/point_xyz.hpp>

namespace boost { namespace geom {

namespace detail {

/// By itself this is useless, it just serve as a _new_ type we
/// specialize point_traits<> with.
///
template<class Host, class XProp, class YProp, class ZProp>
struct point_xyz_wrapper
{
	point_xyz_wrapper(); //Never defined

    private:
	point_xyz_wrapper(const point_xyz_wrapper&);
};

} // namespace detail

// back in namespace boost::geom

// Specializing point_traits<> to deal with point_xyz_wrapper.
template<class Host, class XProp, class YProp, class ZProp>
struct point_traits< detail::point_xyz_wrapper<Host, XProp, YProp, ZProp> >
{	
	/// ????? maybe too restrictive ? maybe unimportant ...
	typedef point_xyz<typename XProp::value_type>  point_type;

    private:
	typedef detail::point_xyz_wrapper<Host, XProp, YProp, ZProp> self_type;
	
	typedef typename point_type::float_type float_type;

	typedef self_type&       reference_type;

	typedef const self_type& param_type;

	static Host& host(reference_type self)
	{ return *reinterpret_cast<Host*>(&self); }

	static const Host& host(param_type self)
	{ return *reinterpret_cast<const Host*>(&self); }

    public:
	static const bool is_specialized=true;	
	struct x 
	{
		typedef typename XProp::value_type value_type;
		typedef typename XProp::ret_type   ret_type;

		static const bool direct_access=XProp::direct_access; 
		
		static void set(reference_type self, value_type v)
		{ XProp::set(host(self),v); }

		static ret_type get(param_type self)
		{ return XProp::get(host(self)); }
		
		static value_type& getset(reference_type self)
		{ return XProp::getset(host(self)); }
	};
	struct y 
	{
		typedef typename YProp::value_type value_type;
		typedef typename YProp::ret_type   ret_type;

		static const bool direct_access=YProp::direct_access; 
		
		static void set(reference_type self, value_type v)
		{ YProp::set(host(self),v); }

		static ret_type get(param_type self)
		{ return YProp::get(host(self)); }
		
		static value_type& getset(reference_type self)
		{ return YProp::getset(host(self)); }
	};
	struct z 
	{
		typedef typename ZProp::value_type value_type;
		typedef typename ZProp::ret_type   ret_type;

		static const bool direct_access=ZProp::direct_access; 
		
		static void set(reference_type self, value_type v)
		{ ZProp::set(host(self),v); }

		static ret_type get(param_type self)
		{ return ZProp::get(host(self)); }
		
		static value_type& getset(reference_type self)
		{ return ZProp::getset(host(self)); }
	};

	struct rho
	{
		typedef float_type value_type;
		typedef value_type ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			value_type factor=v/rho::get(self);
			x::set(self,static_cast<typename x::value_type>(x::get(self)*factor));
			y::set(self,static_cast<typename y::value_type>(y::get(self)*factor));
			z::set(self,static_cast<typename z::value_type>(z::get(self)*factor));
		}
		
		static value_type get(param_type self)
		{
			return std::sqrt(static_cast<value_type>(x::get(self)*x::get(self)+
								 y::get(self)*y::get(self)+
								 z::get(self)*z::get(self)));
		}
		
		static reference_type getset(reference_type self);
	};

	struct theta 
	{
		typedef float_type value_type;
		typedef value_type ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			typename x::value_type x_v=x::get(self);
			typename y::value_type y_v=y::get(self);

			value_type r=std::sqrt(static_cast<value_type>(x_v*x_v+y_v*y_v));
			x::set(self,static_cast<typename x::value_type>(r*std::cos(v)));
			y::set(self,static_cast<typename y::value_type>(r*std::sin(v)));
		}

		static ret_type get(param_type self)
		{
			return std::atan2(static_cast<value_type>(y::get(self)),
					  static_cast<value_type>(x::get(self)));
		}
		

		// No direct access, so not defined ..
		static reference_type getset(reference_type self);
	};

	struct phi
	{
		typedef float_type value_type;
		typedef value_type ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v) // Maybe shouldn't be inlined
		{
			value_type rh=rho::get(self);
			
			value_type th=theta::get(self);
			
			x::set(self,static_cast<typename x::value_type>(rh*std::cos(v)*std::cos(th)));
			y::set(self,static_cast<typename y::value_type>(rh*std::cos(v)*std::sin(th)));
			z::set(self,static_cast<typename z::value_type>(rh*std::sin(v)));
		}

		static value_type get(param_type self)
		{
			// asin (z/rho)
			return std::asin(static_cast<value_type>(z::get(self))/rho::get(self));
					 
		}
		
		static value_type& getset(reference_type self);
	};
	// Operations
	template<class Point>
	static void add(reference_type self, const Point& p)
	{
		x::set(self,x::get(self)+point_traits<Point>::x::get(p));
		y::set(self,y::get(self)+point_traits<Point>::y::get(p));
		z::set(self,z::get(self)+point_traits<Point>::z::get(p));
	}
	
	template<class Point>
	static void sub(reference_type self, const Point& p)
	{
		x::set(self,x::get(self)-point_traits<Point>::x::get(p));
		y::set(self,y::get(self)-point_traits<Point>::y::get(p));
		z::set(self,z::get(self)-point_traits<Point>::z::get(p));
	}
	
	template<class BuiltIn>
	static void mul(reference_type self, BuiltIn p)
	{
		x::set(self,x::get(self)*p);
		y::set(self,y::get(self)*p);
		z::set(self,z::get(self)*p);
	}
	
	template<class BuiltIn>
	static void div(reference_type self, BuiltIn p)
	{
		x::set(self,x::get(self)/p);
		y::set(self,y::get(self)/p);
		z::set(self,z::get(self)/p);
	}

	static void neg(reference_type self)
	{
		x::set(self,-x::get(self));
		y::set(self,-y::get(self));
		z::set(self,-z::get(self));
	}
	
	template<class Point>
	static bool equal(param_type self, const Point& pt)
	{
		return x::get(self)==point_traits<Point>::x::get(pt) &&
		    y::get(self)==point_traits<Point>::y::get(pt) &&
		    z::get(self)==point_traits<Point>::z::get(pt);
	}

	template<class Point>
	static void assign(reference_type self, const Point& p)
	{
		x::set(self,point_traits<Point>::x::get(p));
		y::set(self,point_traits<Point>::y::get(p));
		z::set(self,point_traits<Point>::z::get(p));
	}
};


} } // namespace boost::geom



#endif // #ifndef BOOST_GEOM_DETAIL_POINT_XYZ_WRAPPER
