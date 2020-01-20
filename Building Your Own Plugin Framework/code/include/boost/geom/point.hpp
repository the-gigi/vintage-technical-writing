// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// point.hpp:
//      - Point wrapper class (geom::point<>)
//      - Function wrapper point_wrap()
//      - Specialization of geom::point_impl for geom::point

#ifndef BOOST_GEOM_POINT_HPP
#define BOOST_GEOM_POINT_HPP

// Boost
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/mpl/if.hpp> // for boost::mpl::if_c

// Boost.Geom
#include <boost/geom/detail/builtin_wrapper.hpp>
#include <boost/geom/point_traits.hpp>

namespace boost { namespace geom {

namespace detail {

// Typo helper
template<class Point>
struct point_traits_type
{
	typedef point_traits<typename point_impl<Point>::type > type;
	
};

// Typo helper.
//  if(point_traits<Impl1>::point_type==Impl1)
//     return Impl1
//  else return point_traits<Impl2>::point_type
template<class Impl1, class Impl2>
struct point_ret_type
{
	// if (point_traits<Impl1>::type == Impl1) then type=Impl1
	// else type=Impl2

	typedef typename boost::mpl::if_c< boost::is_same< typename
		point_traits_type<Impl1>::type::point_type ,/*==*/ Impl1 >::value,
					   /*then*/ Impl1,
					   /*else*/ typename point_traits_type<Impl2>::type::point_type 
	>::type type;
};

} // namespace detail

template<class P> class point;

/// This partial specialization tells how to extract the
/// implementation information (type and object) from a
/// point<PointImpl> (which is a wrapper around PointImpl).
template<typename PointImpl>
struct point_impl< point<PointImpl> >
{
	// Simply forward the call to point_impl<PointImpl> class
	
	typedef typename point_impl<PointImpl>::type type;
	
	static type& extract(point<PointImpl>& pt)
	{ return point_impl<PointImpl>::extract(pt.impl()); }
	
	static const type& extract(const point<PointImpl>& pt)
	{ return point_impl<PointImpl>::extract(pt.impl()); }
};

/// Class point is a wrapper around point_traits interface.
///
/// Class point provides:
/// 	- Convinient (OO-like syntax) accessors: e.g pt.x(), pt.rho()=10;
///     - Handy operator overloads: pt+=pt2; pt=-pt*3;
///     - Access to the implementation object (through point::impl())
///     - Mixed-implementation computations, with STATIC_ASSERTs
/// 		preventing you from using everything as a point.
///
/// PointImpl is the implementation object to use.  Any type for which
/// point_traits< point_impl<PointImpl>::type >::is_specialized will do.
///
/// Class point itself can be used as a PointImpl
template<typename PointImpl>
class point {
	typedef typename point_impl<PointImpl>::type impl_type;
	typedef point_traits<impl_type> traits_type;
	
	BOOST_STATIC_ASSERT(traits_type::is_specialized);
	
	PointImpl m_impl;

	impl_type& impl_ref()
	{ return point_impl<PointImpl>::extract(this->m_impl); }

	const impl_type& impl_ref() const
	{ return point_impl<PointImpl>::extract(this->m_impl); }

    public:
	point()
	{}
	
	template<class Impl>
	point(const Impl& im)
	{ this->operator=(im); }
	
	point(const point& im)
	{ this->operator=(im); }
	
	PointImpl& impl() 
	{ return m_impl; }

	const PointImpl& impl() const
	{ return m_impl; }
	
// Setters, pt.set_y(10), pt.set_theta(20); ..etc	
#define BOOST_GEOM_POINT_SET(prop) \
	void set_ ## prop(typename traits_type:: prop :: value_type v) \
	{ \
		traits_type:: prop ::set(this->impl_ref(), v); \
	}
BOOST_GEOM_POINT_SET(x) BOOST_GEOM_POINT_SET(y) BOOST_GEOM_POINT_SET(z) BOOST_GEOM_POINT_SET(rho)
BOOST_GEOM_POINT_SET(theta) BOOST_GEOM_POINT_SET(phi)
#undef BOOST_GEOM_POINT_SET
	
// Getters: pt.get_y(), pt.get_z() .. etc
#define BOOST_GEOM_POINT_GET(prop) \
	typename traits_type:: prop ::value_type \
	get_ ## prop () const \
	{ return traits_type:: prop ::get(this->impl_ref()); }
		
	BOOST_GEOM_POINT_GET(x) BOOST_GEOM_POINT_GET(y) BOOST_GEOM_POINT_GET(z)
		BOOST_GEOM_POINT_GET(rho) BOOST_GEOM_POINT_GET(phi) BOOST_GEOM_POINT_GET(theta)
#undef BOOST_GEOM_POINT_GET

	
// Property accessors: pt.x()=pt.y()+3 value=pt.z()*pt.x() ... etc		
#define BOOST_GEOM_POINT_GETSET(prop) \
typename detail::getset_builtin< typename traits_type:: prop, impl_type>::type \
		prop () \
	{ \
	        return detail::getset_builtin< typename traits_type::prop,\
		    impl_type>::perform(this->impl_ref()); \
	} \
\
	typename traits_type:: prop ::value_type prop() const \
 	{ return this->get_ ## prop ();}
BOOST_GEOM_POINT_GETSET(x) BOOST_GEOM_POINT_GETSET(y) BOOST_GEOM_POINT_GETSET(z) BOOST_GEOM_POINT_GETSET(rho)
	BOOST_GEOM_POINT_GETSET(phi) BOOST_GEOM_POINT_GETSET(theta)
#undef BOOST_GEOM_POINT_GETSET

// Operators
	template<class Point>
	point<typename detail::point_ret_type<PointImpl, Point>::type>
	operator+(const Point& p) const
	{
		BOOST_STATIC_ASSERT(point_traits<typename point_impl<Point>::type>::is_specialized);
		
		typename detail::point_ret_type<Point, PointImpl>::type tmp;
		
		point_wrap(tmp)=*this;
		return point_wrap(tmp)+=p;
	}

	template<class Point>
	point<typename detail::point_ret_type<PointImpl, Point>::type>
	operator-(const Point& p) const
	{
		BOOST_STATIC_ASSERT(point_traits<typename point_impl<Point>::type>::is_specialized);
		
		typename detail::point_ret_type<PointImpl, Point>::type tmp;
		
		point_wrap(tmp)=*this;
		return point_wrap(tmp)-=p;
	}

	template<class BuiltIn>
	point<typename traits_type::point_type>
	operator*(const BuiltIn& p) const
	{
		typename traits_type::point_type tmp;
		
		point_wrap(tmp)=*this;
		return point_wrap(tmp)*=p;
	}

	template<class BuiltIn>
	point<typename traits_type::point_type>
	operator/(const BuiltIn& p) const
	{
		typename traits_type::point_type tmp;
		
		point_wrap(tmp)=*this;
		return point_wrap(tmp)/=p;
	}

	point<typename traits_type::point_type>
	operator-() const
	{
		typedef typename traits_type::point_type ret_type;
		
		typename traits_type::point_type tmp;
		point_wrap(tmp)=*this;
		point_traits<typename point_impl< ret_type >::type>::neg(
			point_impl<ret_type>::extract(tmp));
		return tmp;
	}

	template<class Point>
	bool operator==(const Point& pt) const
	{
		typedef typename point_impl<Point>::type impl_type2;
		
		BOOST_STATIC_ASSERT(point_traits<impl_type2>::is_specialized);
		
		return traits_type::equal(this->impl_ref(), point_impl<Point>::extract(pt));
	}

	template<class Point>
	bool operator!=(const Point& pt) const
	{
		return !(this->operator==(pt));
	}

	////////
	point& operator=(const point& pt)
	{
		traits_type::assign(this->impl_ref(), pt.impl_ref());
		return *this;
	}


	template<class Point>
	point& operator=(const Point& pt)
	{
		typedef point_impl<Point> impl_point;
		typedef typename impl_point::type impl_type2;
		
		BOOST_STATIC_ASSERT(point_traits<impl_type2>::is_specialized);
		
		traits_type::assign(this->impl_ref(), impl_point::extract(pt));
		return *this;
	}
	
	
	template<class Point>
	point& operator+=(const Point& pt)
	{
		typedef point_impl<Point> impl_point;
		typedef typename impl_point::type impl_type2;
		
		BOOST_STATIC_ASSERT(point_traits<impl_type2>::is_specialized);
		
		traits_type::add(this->impl_ref(), impl_point::extract(pt));
		return *this;
	}
	
	template<class Point>
	point& operator-=(const Point& pt)
	{
		typedef point_impl<Point> impl_point;
		typedef typename impl_point::type impl_type2;
		
		BOOST_STATIC_ASSERT(point_traits<impl_type2>::is_specialized);
		
		traits_type::sub(this->impl_ref(), impl_point::extract(pt));
		return *this;
	}
	

	template<class BuiltIn>
	point& operator*=(const BuiltIn& b)
	{
		traits_type::mul(this->impl_ref(), b);
		return *this;
	}

	template<class BuiltIn>
	point& operator/=(const BuiltIn& b)
	{
		traits_type::div(this->impl_ref(), b);
		return *this;
	}
	
}; // class point

/// Turns a PointImpl into a point<PointImpl>
/// (this is a zero cost operation)
template<typename PointImpl>
typename boost::mpl::if_c<boost::is_const<PointImpl>::value,
			  /*then*/const point<typename boost::remove_const<PointImpl>::type>,
			  /*else*/point<PointImpl> >::type &
point_wrap(PointImpl& impl)
{
	return *reinterpret_cast<typename boost::mpl::if_c<boost::is_const<PointImpl>::value,
		 const point<typename boost::remove_const<PointImpl>::type >,
	    point<PointImpl> >::type* > (&impl);
}
	
} }  // namespace boost::geom

#endif // #ifndef BOOST_GEOM_POINT_HPP
