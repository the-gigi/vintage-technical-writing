// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// box_xywh.hpp:
// 	- Implementation box_xywh<>
//	- Specialization of box_traits<> for class box_xywh<>

#ifndef BOOST_GEOM_BOX_XYWH_HPP
#define BOOST_GEOM_BOX_XYWH_HPP

//--Standard C++----------------------------------------------------------------
#include <algorithm> // for min&max, costy but ...

//--Boost-----------------------------------------------------------------------
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_same.hpp>

//--Geom------------------------------------------------------------------------
#include <boost/geom/box_traits.hpp>
#include <boost/geom/box_xywh_traits.hpp>

namespace boost { namespace geom {

/// Box implementation. Holds its x, y, width and height as data
/// fields of type ``Value''. The z is always zero and the depth is
/// always one, i.e. these are flat boxes layed on the z=0 plan.
template<typename Value>
struct box_xywh
{
	typedef Value value_type;
	value_type x, y, width, height;

	box_xywh()
	{}

	box_xywh(value_type mx, value_type my, value_type mw, value_type mh):
		x(mx), y(my), width(mw), height(mh)
	{}
	
};

namespace detail {

template<typename Value>
struct box_xywh_prop
{
	typedef box_xywh<Value>& reference_type;
	typedef const box_xywh<Value>& param_type;
	
	struct x1  
	{
		// X1 Is actually the ``x'' field of box_xywh
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.x=v; }

		static ret_type get(param_type self)
		{ return self.x; }
		
		static value_type& getset(reference_type self)
		{ return self.x; }
		
	};
	struct y1
	{
		// Y1 Is actually the ``y'' field of box_xywh
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.y=v; }

		static ret_type get(param_type self)
		{ return self.y; }
		
		static value_type& getset(reference_type self)
		{ return self.y; }
		
	};

	struct width
	{
		typedef Value      value_type;
		typedef value_type ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.width=v; }

		static value_type get(param_type self)
		{ return self.width; }

		static value_type& getset(reference_type self)
		{ return self.width; }
	};
	
	struct height
	{
		typedef Value      value_type;
		typedef value_type ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.height=v; }

		static value_type get(param_type self)
		{ return self.height; }

		static value_type& getset(reference_type self)
		{ return self.height; }
	};
}; // struct box_xywh_prop
 
} // namespace detail


/// Specializing box_traits to deal with  box_xywh
template<typename Value>
struct box_traits< box_xywh<Value> >:
	public box_xywh_traits<box_xywh<Value>,
			       typename detail::box_xywh_prop<Value>::x1,
			       typename detail::box_xywh_prop<Value>::y1,
			       typename detail::box_xywh_prop<Value>::width,
			       typename detail::box_xywh_prop<Value>::height>
			       
{
	static const bool is_specialized=true;
};


} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_BOX_XYWH_HPP
