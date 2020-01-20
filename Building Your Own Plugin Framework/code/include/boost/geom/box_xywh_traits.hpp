// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// box_xywh_traits.hpp:
// 	- Traits generator box_xywh_traits<>

#ifndef BOOST_GEOM_BOX_XYWH_TRAITS_HPP
#define BOOST_GEOM_BOX_XYWH_TRAITS_HPP

// Standard C++
#include <algorithm> // for min&max, costy but ...

// Boost.Geom
#include <boost/geom/point_traits.hpp>

namespace boost { namespace geom {

// Defines box properties based on a subset of them, namely X1, Y1,
// Width and Height, (Z1 is always zero and Depth is always 1).
// This is intended to help specialize box_traits.
// Host: The box_type hosting the properties
/// The value_type of the created properties will be typename X1::value_type
template<class Host, class X1, class Y1, class Width, class Height>
struct box_xywh_traits
{
    private:	
	typedef Host&       reference_type;
	typedef const Host& param_type;

    public:
	typedef Host        box_type;
	
	typedef X1 x1;
	typedef Y1 y1;
	typedef Width  width;
	typedef Height height;
	
	static const bool is_specialized=true;

	struct xmin  
	{
		typedef typename X1::value_type        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			width::set(self, width::get(self)+x1::get(self)-v);
			x1::set(self, v);
		}

		static ret_type get(param_type self)
		{ return x1::get(self); }
		
		static value_type& getset(reference_type self);
	};

	struct ymin  
	{
		typedef typename X1::value_type     value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			height::set(self, height::get(self)+y1::get(self)-v);
			y1::set(self, v);
		}

		static ret_type get(param_type self)
		{ return y1::get(self); }
		
		static value_type& getset(reference_type self);
	};

	struct xmax
	{
		typedef typename X1::value_type        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			width::set(self, v-x1::get(self)+1);
		}

		static ret_type get(param_type self)
		{ return x1::get(self)+width::get(self)-1; }
		
		static value_type& getset(reference_type self);
	};

	struct ymax
	{
		typedef typename X1::value_type        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			height::set(self, v-y1::get(self)+1);
		}

		static ret_type get(param_type self)
		{ return y1::get(self)+height::get(self)-1; }
		
		static value_type& getset(reference_type self);
	};
	
	struct x2
	{
		typedef typename X1::value_type        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			x1::set(self, v-width::get(self)+1);
		}

		static ret_type get(param_type self)
		{ return x1::get(self)+width::get(self)-1; }
		
		static value_type& getset(reference_type self);
	};

	struct y2
	{
		typedef typename X1::value_type        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{
			y1::set(self, v-height::get(self)+1);
		}

		static ret_type get(param_type self)
		{ return y1::get(self)+height::get(self)-1; }
		
		static value_type& getset(reference_type self);
	};
	
	// The Z1, Z2, Zmin, and Zmax properties of an XYWH box are
	// always zero.  Depth is always one.  See
	// geom/detail/dummy_property.hpp for a definition of
	// detail::fixed_property<>
	typedef detail::fixed_property<box_type, typename X1::value_type, 0> z1;
	typedef detail::fixed_property<box_type, typename X1::value_type, 0> z2;
	typedef detail::fixed_property<box_type, typename X1::value_type, 0> zmin;
	typedef detail::fixed_property<box_type, typename X1::value_type, 0> zmax;
	typedef detail::fixed_property<box_type, typename X1::value_type, 1> depth;	
	
	// Operations
	template<class Point>
	static void add(reference_type self, const Point& pt)
	{
		x1::set(self, x1::get(self) + point_traits<Point>::x::get(pt));
		y1::set(self, y1::get(self) + point_traits<Point>::x::get(pt));
	}
	
	template<class Point>
	static void sub(reference_type self, const Point& pt)
	{
		x1::set(self, x1::get(self) - point_traits<Point>::x::get(pt));
		y1::set(self, y1::get(self) - point_traits<Point>::x::get(pt));
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
		return x1::get(self)==traits::x1::get(bx) &&
		    y1::get(self)==traits::y1::get(bx) &&
		    width::get(self)==traits::width::get(bx) &&
		    height::get(self)==traits::height::get(bx) &&
		    0==traits::z1::get(bx) &&
		    1==traits::depth::get(bx);
	}
	
	static bool valid(param_type self)
	{ return width::get(self)>0 && height::get(self)>0; }

	template<class Box>
	static void assign(reference_type self, const Box& bx)
	{
		x1::set(self, box_traits<Box>::x1::get(bx));
		y1::set(self, box_traits<Box>::y1::get(bx));
		width::set(self, box_traits<Box>::width::get(bx));
		height::set(self, box_traits<Box>::height::get(bx));
	}
};

} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_BOX_XYWH_TRAITS_HPP
