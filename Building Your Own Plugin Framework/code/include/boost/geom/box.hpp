// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// box.hpp:
// 	- Box wrapper class (geom::box<>)
//      - Function wrapper box_wrap()
//	- Symbolic constants for corner specifiers (xmin, xmax, etc)
//      - Specialization of geom::box_impl for geom::box

#ifndef BOOST_GEOM_BOX_HPP
#define BOOST_GEOM_BOX_HPP

// Boost
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/mpl/if.hpp> // for boost::mpl::if_c

// Boost.Geom
#include <boost/geom/detail/point_xyz_wrapper.hpp>
#include <boost/geom/point.hpp>
#include <boost/geom/box_traits.hpp>

namespace boost { namespace geom {

/// The follwing three enums are used to specify a "corner" of a box
enum x_corner
{ xmin, xmax, x1, x2 };

enum y_corner
{ ymin, ymax, y1, y2 };

// z_corner_dummy is used as a default value when the z_corner
// specifier is actually omitted, this will allow the implementation to
// choose between zmin and z1 based on the x_corner and y_corner specifiers.
enum z_corner
{ zmin, zmax, z1, z2, z_corner_dummy };

namespace detail {

/// Template meta function returning the (three) properties (in Traits) 
/// assiciated with the given corner specifier
template<class Traits, x_corner xc, y_corner yc, z_corner zc>
struct corner_prop
{
    private:
	/// X
	typedef typename boost::mpl::if_c<xc==x1,
					  typename Traits::x1, 
					  typename Traits::x2>::type test_for_x1x2;
	
	typedef typename boost::mpl::if_c<xc==xmax,
					  typename Traits::xmax,
					  test_for_x1x2>::type test_for_xmax_x1x2;
	/// Y
	typedef typename boost::mpl::if_c<yc==y1,
					  typename Traits::y1, 
					  typename Traits::y2>::type test_for_y1y2;
	
	typedef typename boost::mpl::if_c<yc==ymax,
					  typename Traits::ymax,
					  test_for_y1y2>::type test_for_ymax_y1y2;
	/// Z
	typedef typename boost::mpl::if_c<zc==z1,
					  typename Traits::z1, 
					  typename Traits::z2>::type test_for_z1z2;
	
	typedef typename boost::mpl::if_c<zc==zmax,
					  typename Traits::zmax,
					  test_for_z1z2>::type test_for_zmax_z1z2;

    public:
	// This works like this,
	// if(xc==xmin) return Traits::xmin
	// else if(xc==xmax) return Traits::xmax
	// else if(xc==x1) return Traits::x1
	// else return Traits::x2
	// Same goes for y and z.
	typedef typename boost::mpl::if_c<xc==xmin,
					  typename Traits::xmin,
					  test_for_xmax_x1x2>::type xprop;
	typedef typename boost::mpl::if_c<yc==ymin,
					  typename Traits::ymin,
					  test_for_ymax_y1y2>::type yprop;
	typedef typename boost::mpl::if_c<zc==zmin,
					  typename Traits::zmin,
					  test_for_zmax_z1z2>::type zprop;
};

/// Template meta-function that returns the ``z_corner" specifier
/// corresponding to given x and y corner specifiers.
/// 
/// This works like this:
/// if (zc!=z_corner_dummy) {
///      (this means the user explicitly specified a z_corner specifier)
///      return zc;
/// } else {
///    (this means the user relied on the default z_corner specifier)
///    if(xc is (xmin or xmax) && yc is (ymin or ymax)) return zmin;
///    if(xc is (x1 or x2) && yc is (y1 or y2)) return z1;
///    else trigger a compile time error (no default value can be
///     given)
///  }
template<x_corner xc, y_corner yc, z_corner zc>
struct default_z_corner
{
	// This definition is used if zc!=zc_dummy_corner
	static z_corner const value=zc; // Nop !!
};

template<x_corner xc, y_corner yc>
struct default_z_corner<xc,yc,z_corner_dummy>; //  Not defined: here is the compile time error

#define BOOST_GEOM_DETAIL_DEFAULT_Z_CORNER(xc, yc, zc)\
template<>					\
struct default_z_corner<xc, yc, z_corner_dummy>	\
{						\
	static const z_corner value=zc;         \
}

BOOST_GEOM_DETAIL_DEFAULT_Z_CORNER(xmin, ymin, zmin);// if(xc==xmin&&yc==ymin) return zmin ... etc
BOOST_GEOM_DETAIL_DEFAULT_Z_CORNER(xmax, ymin, zmin);
BOOST_GEOM_DETAIL_DEFAULT_Z_CORNER(xmin, ymax, zmin);
BOOST_GEOM_DETAIL_DEFAULT_Z_CORNER(xmax, ymax, zmin);

BOOST_GEOM_DETAIL_DEFAULT_Z_CORNER(x1, y1, z1);
BOOST_GEOM_DETAIL_DEFAULT_Z_CORNER(x2, y1, z1);
BOOST_GEOM_DETAIL_DEFAULT_Z_CORNER(x1, y2, z1);
BOOST_GEOM_DETAIL_DEFAULT_Z_CORNER(x2, y2, z1);

/// The following meta-function combines default_z_corner with corner_prop to return
/// a point_xyz_wrapper corresponding to the given Box type and corner
/// specifiers
template<class BoxImpl, x_corner xc, y_corner yc, z_corner zc>
struct corner_wrapper
{
    private:
	typedef BoxImpl impl_type;
	typedef box_traits< impl_type > traits_type;

	static const z_corner def_zc=default_z_corner<xc,yc,zc>::value;
	
    public:
	typedef point_xyz_wrapper<impl_type,
				 typename corner_prop<traits_type, xc, yc, def_zc>::xprop,
				 typename corner_prop<traits_type, xc, yc, def_zc>::yprop,
				 typename corner_prop<traits_type, xc, yc, def_zc>::zprop> type;
};

/// This will generate a ``property" (in the point_traits sens of the
/// term) representing PosProp+LengthProp/2. Where PosProp will be
/// traits::x1, traits::y1 or traits::z1, and LengthProp will be
/// (resp) traits::width, traits::height or traits::depth. This is
/// used to implement the center() property of boxes.
template<class BoxImpl, class PosProp, class LengthProp>
struct center_prop
{
    private:
	typedef BoxImpl impl_type;
	
    public:
	typedef typename PosProp::value_type value_type;
	typedef typename PosProp::ret_type   ret_type;

	static const bool direct_access=false;
	
	static void set(impl_type& bx, value_type v)
	{ PosProp::set(bx, v-LengthProp::get(bx)/2); }

	static value_type get(const impl_type& bx)
	{ return PosProp::get(bx)+LengthProp::get(bx)/2; }

	static value_type& getset(impl_type&);
};	


template<class Box>
struct box_traits_type
{
	typedef box_traits<typename box_impl<Box>::type > type;
	
};

template<class Impl1, class Impl2>
struct box_ret_type
{
	// if (box_traits<Impl1>::type == Impl1) then type=Impl1
	// else type=Impl2

	typedef typename boost::mpl::if_c< boost::is_same< typename
		box_traits_type<Impl1>::type::box_type ,/*==*/ Impl1 >::value,
					   /*then*/ Impl1,
					   /*else*/ typename box_traits_type<Impl2>::type::box_type 
	>::type type;
};

} // namespace detail

// In namespace boost::geom

template<class BoxImpl> class box;

/// Turns a BoxImpl into a box<BoxImpl>
/// (this is a zero cost operation)
template<typename BoxImpl>
typename boost::mpl::if_c<boost::is_const<BoxImpl>::value,
			  /*then*/const box<typename boost::remove_const<BoxImpl>::type>,
			  /*else*/box<BoxImpl> >::type &
box_wrap(BoxImpl& impl)
{
	return *reinterpret_cast<typename boost::mpl::if_c<boost::is_const<BoxImpl>::value,
		 const box<typename boost::remove_const<BoxImpl>::type >,
	    box<BoxImpl> >::type* > (&impl);
} 

/// Class box is a wrapper around box_traits interface.
///
/// Class box provides:
/// 	- Convinient (OO-like syntax) accessorse: e.g bx.xmin(), bx.width()=10;
///     - Handy operator overloads: bx&=bx2, bx==bx2, bx+=pt; ..etc
///     - Access to the implementation object (through box::impl())
///     - Mixed-implementation computations, with STATIC_ASSERTs
/// 		preventing you from using everything as a box or point.
///
/// BoxImpl is the implementation object to use.  Any type for which
/// box_traits< box_impl<BoxImpl>::type >::is_specialized will do.
///
/// Class box itself can be used as a BoxImpl
template<typename BoxImpl>
class box {
	typedef typename box_impl<BoxImpl>::type impl_type;
	typedef box_traits<impl_type>            traits_type;
	
	BOOST_STATIC_ASSERT(traits_type::is_specialized);
	
	BoxImpl m_impl;

	impl_type& impl_ref()
	{ return box_impl<BoxImpl>::extract(this->m_impl); }
	
	const impl_type& impl_ref() const
	{ return box_impl<BoxImpl>::extract(this->m_impl); }
	
    public:
	box()
	{}

	template<class Impl>
	box(const Impl& im)
	{ this->operator=(im); }
	
	box(const box& im)
	{ this->operator=(im); }
	
	BoxImpl& impl() 
	{ return m_impl; }

	const BoxImpl& impl() const
	{ return m_impl; }

// Getters: bx.get_y(), bx.get_depth() .. etc
#define BOOST_GEOM_BOX_GET(prop) \
	typename traits_type:: prop ::value_type \
	get_ ## prop () const \
	{ return traits_type::prop::get(this->impl_ref()); }

	BOOST_GEOM_BOX_GET(x1) BOOST_GEOM_BOX_GET(y1) BOOST_GEOM_BOX_GET(z1)
		BOOST_GEOM_BOX_GET(x2) BOOST_GEOM_BOX_GET(y2) BOOST_GEOM_BOX_GET(z2)
	BOOST_GEOM_BOX_GET(xmin) BOOST_GEOM_BOX_GET(ymin) BOOST_GEOM_BOX_GET(zmin)
		BOOST_GEOM_BOX_GET(xmax) BOOST_GEOM_BOX_GET(ymax) BOOST_GEOM_BOX_GET(zmax)
		BOOST_GEOM_BOX_GET(width) BOOST_GEOM_BOX_GET(height) BOOST_GEOM_BOX_GET(depth)
#undef BOOST_GEOM_BOX_GET


// Setters: bx.set_y(value), bx.set_width(value) .. etc		
#define BOOST_GEOM_BOX_SET(prop) \
	void set_ ## prop (typename traits_type:: prop ::value_type v) \
	{ traits_type::prop::set(this->impl_ref(), v);}

	BOOST_GEOM_BOX_SET(x1) BOOST_GEOM_BOX_SET(y1) BOOST_GEOM_BOX_SET(z1)
		BOOST_GEOM_BOX_SET(x2) BOOST_GEOM_BOX_SET(y2) BOOST_GEOM_BOX_SET(z2)
		BOOST_GEOM_BOX_SET(xmin) BOOST_GEOM_BOX_SET(ymin) BOOST_GEOM_BOX_SET(zmin)
		BOOST_GEOM_BOX_SET(xmax) BOOST_GEOM_BOX_SET(ymax) BOOST_GEOM_BOX_SET(zmax)
		BOOST_GEOM_BOX_SET(width) BOOST_GEOM_BOX_SET(height) BOOST_GEOM_BOX_SET(depth)
#undef BOOST_GEOM_BOX_SET

// Property accessors: bx.width()=bx.height()+3 value=bx.xmax()*bx.xmin() ... etc		
#define BOOST_GEOM_BOX_GETSET(prop) \
typename detail::getset_builtin< typename traits_type:: prop, impl_type>::type \
		prop () \
	{ return detail::getset_builtin<typename traits_type::prop, impl_type>::perform(this->impl_ref()); } \
\
	typename traits_type:: prop ::value_type prop() const \
 	{ return this->get_ ## prop ();}
BOOST_GEOM_BOX_GETSET(x1) BOOST_GEOM_BOX_GETSET(y1) BOOST_GEOM_BOX_GETSET(z1)
	BOOST_GEOM_BOX_GETSET(x2) BOOST_GEOM_BOX_GETSET(y2) BOOST_GEOM_BOX_GETSET(z2)
	BOOST_GEOM_BOX_GETSET(xmin) BOOST_GEOM_BOX_GETSET(ymin) BOOST_GEOM_BOX_GETSET(zmin)
	BOOST_GEOM_BOX_GETSET(xmax) BOOST_GEOM_BOX_GETSET(ymax) BOOST_GEOM_BOX_GETSET(zmax)
	BOOST_GEOM_BOX_GETSET(width) BOOST_GEOM_BOX_GETSET(height) BOOST_GEOM_BOX_GETSET(depth)
#undef BOOST_GEOM_BOX_GETSET

	bool valid() const
	{ return traits_type::valid(this->impl_ref()); }
	
// Operators
	
	template<class Box>
	box& operator=(const Box& bx)
	{
		typedef typename box_impl<Box>::type impl_type2;
		typedef box_impl<Box>                impl_box;

		BOOST_STATIC_ASSERT(box_traits<impl_type2>::is_specialized);
		
		traits_type::assign(this->impl_ref(), impl_box::extract(bx));
		return *this;
	}
	
	box& operator=(const box& bx)
	{
		traits_type::assign(impl_ref(), bx.impl_ref());
		return *this;
	}

	template<class Point>
	box<typename traits_type::box_type>
	operator+(const Point& p) const
	{
		typename traits_type::box_type tmp;
		box_wrap(tmp)=*this;
		return box_wrap(tmp)+=p;
	}

	template<class Point>
	box<typename traits_type::box_type>
	operator-(const Point& p) const
	{
		typename traits_type::box_type tmp;
		box_wrap(tmp)=*this;
		return box_wrap(tmp)-=p;
	}
	
	template<class Box>
	box<typename detail::box_ret_type<BoxImpl, Box>::type>
	operator&(const Box& bx) const
	{
		typename detail::box_ret_type<BoxImpl, Box>::type tmp;
		box_wrap(tmp)=*this;
		return box_wrap(tmp)&=bx;
	}

	template<class Box>
	box<typename detail::box_ret_type<BoxImpl, Box>::type>
	operator|(const Box& bx) const
	{
		typename detail::box_ret_type<BoxImpl, Box>::type tmp;
		box_wrap(tmp)=*this;
		return box_wrap(tmp)|=bx;
	}
	
	////////

	template<class Box>
	bool operator==(const Box& bx) const
	{
		typedef typename box_impl<Box>::type impl_type2;
		typedef box_impl<Box>                impl_box;

		BOOST_STATIC_ASSERT(box_traits<impl_type2>::is_specialized);
		
		return traits_type::equal(this->impl_ref(), impl_box::extract(bx));
	}

	template<class Box>
	bool operator!=(const Box& bx) const
	{ return !this->operator==(bx);}

	template<class Point>
	box& operator+=(const Point& pt)
	{
		typedef typename point_impl<Point>::type impl_type2;
		typedef point_impl<Point>                impl_point;

		BOOST_STATIC_ASSERT(point_traits<impl_type2>::is_specialized);
		
		traits_type::add(this->impl_ref(), impl_point::extract(pt));
		return *this;
	}

	template<class Point>
	box& operator-=(const Point& pt)
	{
		typedef typename point_impl<Point>::type impl_type2;
		typedef point_impl<Point>                impl_point;

		BOOST_STATIC_ASSERT(point_traits<impl_type2>::is_specialized);
		
		traits_type::sub(this->impl_ref(), impl_point::extract(pt));
		return *this;
	}
	

	template<class Box>
	box& operator&=(const Box& b)
	{
		typedef typename box_impl<Box>::type impl_type2;
		typedef box_impl<Box>                impl_box;

		BOOST_STATIC_ASSERT(box_traits<impl_type2>::is_specialized);
		
		traits_type::and_op(this->impl_ref(), impl_box::extract(b));
		return *this;
	}
	

	template<class Box>
	box& operator|=(const Box& b)
	{
		typedef typename box_impl<Box>::type impl_type2;
		typedef box_impl<Box>                impl_box;

		BOOST_STATIC_ASSERT(box_traits<impl_type2>::is_specialized);
				
		traits_type::or_op(this->impl_ref(), impl_box::extract(b));
		return *this;
	}

	///////
	// Corner accessors
	template<x_corner xc, y_corner yc, z_corner zc>
	point<typename detail::corner_wrapper<impl_type, xc, yc, zc>::type>&
	corner()
	{
		return *reinterpret_cast<
		    point<typename detail::corner_wrapper<impl_type,xc,yc,zc>::type>*
		    > (&this->impl_ref());
	}
	
	template<x_corner xc, y_corner yc, z_corner zc>
	const point<typename detail::corner_wrapper<impl_type, xc, yc, zc>::type>&
	corner() const
	{
		return *reinterpret_cast<
		    const
		    point<typename detail::corner_wrapper<impl_type,xc,yc,zc>::type>*
		    > (&this->impl_ref());
	}

	/// Now with default z corner
	template<x_corner xc, y_corner yc>
	point<typename detail::corner_wrapper<impl_type,xc,yc,z_corner_dummy>::type>&
	corner()
	{
		return this->corner<xc, yc, z_corner_dummy>();
	}
	
	template<x_corner xc, y_corner yc>
	const
	point<typename detail::corner_wrapper<impl_type,xc,yc,z_corner_dummy>::type>&
	corner() const
	{
		return this->corner<xc, yc, z_corner_dummy>();
	}

	////
	// Center
    private:
	typedef point<detail::point_xyz_wrapper<impl_type,
				  detail::center_prop<impl_type, typename traits_type::x1, typename traits_type::width>,
				  detail::center_prop<impl_type, typename traits_type::y1, typename traits_type::height>,
				  detail::center_prop<impl_type, typename traits_type::z1, typename traits_type::depth> > >
	center_wrapper;
	
    public:
	center_wrapper& center(){
		return *reinterpret_cast<center_wrapper*>(&(this->impl_ref()));
	}
	
	const center_wrapper& center() const{
		return *reinterpret_cast<const center_wrapper*>(&(this->impl_ref()));
	}
	

}; // class box

/// This partial specialization tells how to extract the
/// implementation information (type and object) from a
/// box<BoxImpl> (which is a wrapper around BoxImpl).
template<typename BoxImpl>
struct box_impl< box<BoxImpl> >
{
	// Simply forward the call to box_impl<BoxImpl>
	
	typedef typename box_impl<BoxImpl>::type type;

	static type& extract(box<BoxImpl>& bx)
	{ return box_impl<BoxImpl>::extract(bx.impl()); }
	
	static const type& extract(const box<BoxImpl>& bx)
	{ return box_impl<BoxImpl>::extract(bx.impl()); }
};
	
} }  // namespace boost::geom

#endif // #ifndef BOOST_GEOM_BOX_HPP
