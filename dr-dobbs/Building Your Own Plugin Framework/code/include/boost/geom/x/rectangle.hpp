// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// x/rectangle.hpp:
//	- Specialization of box_traits<> for XRectangle

#ifndef BOOST_GEOM_X_RECTANGLE_HPP
#define BOOST_GEOM_X_RECTANGLE_HPP

// Boost.Geom
#include <boost/geom/box_traits.hpp>
#include <boost/geom/box_xywh_traits.hpp>

// X
#include <X11/Xlib.h>

namespace boost { namespace geom {

namespace detail {

struct x_rectangle_prop
{
	typedef XRectangle& reference_type;
	typedef const XRectangle& param_type;
	
	typedef short signed_coord;
	typedef unsigned short unsigned_coord;
	
	struct x1  
	{
		typedef signed_coord        value_type;
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
		typedef signed_coord value_type;
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
		typedef signed_coord      value_type;
		typedef value_type ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ reinterpret_cast<signed_coord&>(self.width)=v; }

		static value_type get(param_type self)
		{ return reinterpret_cast<const signed_coord&>(self.width); }

		static value_type& getset(reference_type self)
		{ return reinterpret_cast<signed_coord&>(self.width); }
	};
	
	struct height
	{
		typedef signed_coord    value_type;
		typedef value_type ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ reinterpret_cast<signed_coord&>(self.height)=v; }

		static value_type get(param_type self)
		{ return reinterpret_cast<const signed_coord&>(self.height); }

		static value_type& getset(reference_type self)
		{ return reinterpret_cast<signed_coord&>(self.height); }
	};
}; // struct x_rectangle_prop
 
} // namespace detail


/// Specializing box_traits to deal with XRectangle
template<>
struct box_traits< XRectangle >:
	public box_xywh_traits<XRectangle,
			       detail::x_rectangle_prop::x1,
			       detail::x_rectangle_prop::y1,
			       detail::x_rectangle_prop::width,
			       detail::x_rectangle_prop::height>
			       
{
	static const bool is_specialized=true;
};


} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_X_RECTANGLE_HPP
