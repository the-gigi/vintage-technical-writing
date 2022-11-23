// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// box_lrtb_traits.hpp:
// 	- Traits generator box_lrtb_traits<>

#ifndef BOOST_GEOM_BOX_LRTB_TRAITS_HPP
#define BOOST_GEOM_BOX_LRTB_TRAITS_HPP

//--Standard C++----------------------------------------------------------------
#include <algorithm> // for min&max, costy but ...

#include <boost/geom/point_traits.hpp>
#include <boost/geom/box_traits.hpp>

namespace boost { namespace geom {

// Defines box properties based on a subset of them, namely Xmix/Xmax,
// Ymin/Ymax  ( Zmin&Zmax are supposed to be always zero).
// This is intended to help specialize box_traits.
// Host: The box_type hosting the properties
/// The value_type of the created properties will be typename Xmin::value_type
template<class Host, class Xmin, class Xmax, class Ymin, class Ymax>
struct box_lrtb_traits
{
    private:	
	typedef Host&       reference_type;
	typedef const Host& param_type;

    public:
	typedef Host        box_type;
	
	typedef Xmin xmin;
	typedef Xmax xmax;
	typedef Ymin ymin;
	typedef Ymax ymax;
	
	static const bool is_specialized=true;

	struct x1  
	{
		typedef typename Xmin::value_type        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			xmax::set(self, xmax::get(self)+v-xmin::get(self));
			xmin::set(self, v);
		}

		static ret_type get(param_type self)
		{ return xmin::get(self); }
		
		static value_type& getset(reference_type self);
	};
	
	struct x2
	{
		typedef typename Xmin::value_type        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			xmin::set(self, xmin::get(self)+v-xmax::get(self));
			xmax::set(self, v);
		}

		static ret_type get(param_type self)
		{ return xmax::get(self); }
		
		static value_type& getset(reference_type self);
	};
	struct y1  
	{
		typedef typename Xmin::value_type        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			ymax::set(self, ymax::get(self)+v-ymin::get(self));
			ymin::set(self, v);
		}

		static ret_type get(param_type self)
		{ return ymin::get(self); }
		
		static value_type& getset(reference_type self);
	};
	
	struct y2
	{
		typedef typename Xmin::value_type        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			ymin::set(self, ymin::get(self)+v-ymax::get(self));
			ymax::set(self, v);
		}

		static ret_type get(param_type self)
		{ return ymax::get(self); }
		
		static value_type& getset(reference_type self);
	};

	// The Z1, Z2, Zmin, and Zmax properties of an LRTB box are
	// always zero.  Depth is always one.  See
	// geom/detail/dummy_property.hpp for a definition of
	// detail::fixed_property<>
	typedef detail::fixed_property<box_type, typename Xmin::value_type, 0> z1;
	typedef detail::fixed_property<box_type, typename Xmin::value_type, 0> z2;
	typedef detail::fixed_property<box_type, typename Xmin::value_type, 0> zmin;
	typedef detail::fixed_property<box_type, typename Xmin::value_type, 0> zmax;
	typedef detail::fixed_property<box_type, typename Xmin::value_type, 1> depth;
	
	struct width
	{
		typedef typename Xmin::value_type      value_type;
		typedef value_type ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{ xmax::set(self,v+xmin::get(self)-1); }

		static value_type get(param_type self)
		{ return xmax::get(self)-xmin::get(self)+1; }

		static value_type& getset(reference_type self);
	};
	
	struct height
	{
		typedef typename Xmin::value_type      value_type;
		typedef value_type ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{ ymax::set(self,v+ymin::get(self)-1); }

		static ret_type get(param_type self)
		{ return ymax::get(self)-ymin::get(self)+1; }

		static value_type& getset(reference_type self);
	};
	
	
	// Operations
	template<class Point>
	static void add(reference_type self, const Point& pt)
	{
		xmin::set(self, xmin::get(self) + point_traits<Point>::x::get(pt));
		xmax::set(self, xmax::get(self) + point_traits<Point>::x::get(pt));
		ymin::set(self, ymin::get(self) + point_traits<Point>::y::get(pt));
		ymax::set(self, ymax::get(self) + point_traits<Point>::y::get(pt));
	}
	
	template<class Point>
	static void sub(reference_type self, const Point& pt)
	{
		xmin::set(self, xmin::get(self) - point_traits<Point>::x::get(pt));
		xmax::set(self, xmax::get(self) - point_traits<Point>::x::get(pt));
		ymin::set(self, ymin::get(self) - point_traits<Point>::y::get(pt));
		ymax::set(self, ymax::get(self) - point_traits<Point>::y::get(pt));
	}

	template<class Box>
	static void or_op(reference_type self, const Box& bx)
	{
		typedef box_traits<Box> traits;
		xmin::set(self, (std::min)(xmin::get(self), traits::xmin::get(bx)));
		ymin::set(self, (std::min)(ymin::get(self), traits::ymin::get(bx)));
		xmax::set(self, (std::max)(xmax::get(self), traits::xmax::get(bx)));
		ymax::set(self, (std::max)(ymax::get(self), traits::ymax::get(bx)));
		
	}
	

	template<class Box>
	static void and_op(reference_type self, const Box& bx)
	{
		typedef box_traits<Box> traits;
		xmin::set(self, (std::max)(xmin::get(self), traits::xmin::get(bx)));
		ymin::set(self, (std::max)(ymin::get(self), traits::ymin::get(bx)));
		xmax::set(self, (std::min)(xmax::get(self), traits::xmax::get(bx)));
		ymax::set(self, (std::min)(ymax::get(self), traits::ymax::get(bx)));

	}
	
	template<class Box>
	static bool equal(param_type self, const Box& bx)
	{
		typedef box_traits<Box> traits;
		return xmin::get(self)==traits::xmin::get(bx) &&
		    ymin::get(self)==traits::ymin::get(bx) &&
		    xmax::get(self)==traits::xmax::get(bx) &&
		    ymax::get(self)==traits::ymax::get(bx) &&
		    0==traits::zmin::get(bx) &&
		    0==traits::zmax::get(bx);
	}
	
	static bool valid(param_type self)
	{ return xmax::get(self)>=xmin::get(self) && ymax::get(self)>=ymin::get(self); }

	template<class Box>
	static void assign(reference_type self, const Box& bx)
	{
		xmin::set(self, box_traits<Box>::xmin::get(bx));
		ymin::set(self, box_traits<Box>::ymin::get(bx));
		xmax::set(self, box_traits<Box>::xmax::get(bx));
		ymax::set(self, box_traits<Box>::ymax::get(bx));
	}
};


} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_BOX_LRTB_TRAITS_HPP
