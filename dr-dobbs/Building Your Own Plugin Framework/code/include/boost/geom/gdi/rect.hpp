// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// gdi/rect.hpp:
//      - Specialization of box_traits for RECT

#ifndef BOOST_GEOM_GDI_RECT_HPP
#define BOOST_GEOM_GDI_RECT_HPP

// Windows API
#include <windows.h> // for RECT (isn't there some smaller header ..)

// Boost.Geom
#include <boost/geom/box_lrtb_traits.hpp>
#include <boost/geom/box_traits.hpp>

namespace boost { namespace geom {

namespace detail {

struct gdi_rect_prop
{
	typedef LONG  Value;
	typedef RECT& reference_type;
	typedef const RECT& param_type;
	
	struct xmin 
	{
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.left=v;}

		static ret_type get(param_type self)
		{ return self.left; }

		static value_type& getset(reference_type self)
		{ return self.left; }
	};

	struct xmax 
	{
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{ self.right=v+1; }

		static value_type get(param_type self)
		{ return self.right-1; }

		static value_type& getset(reference_type self);
	};

	struct ymin 
	{
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.top=v;}

		static ret_type get(param_type self)
		{ return self.top; }

		static value_type& getset(reference_type self)
		{ return self.top; }
	};

	struct ymax 
	{
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=false;
		
		static void set(reference_type self, value_type v)
		{ self.bottom=v+1; }

		static value_type get(param_type self)
		{ return self.bottom-1; }

		static value_type& getset(reference_type self);
	};
};

} // namespace detail

// back in namespace boost::geom

/// Specializing box_traits to deal with RECT
template<>
struct box_traits<RECT>:
	public box_lrtb_traits<RECT,
					detail::gdi_rect_prop::xmin,
					detail::gdi_rect_prop::xmax,
					detail::gdi_rect_prop::ymin,
					detail::gdi_rect_prop::ymax>

{ static const bool is_specialized=true; };


} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_GDI_RECT_HPP
