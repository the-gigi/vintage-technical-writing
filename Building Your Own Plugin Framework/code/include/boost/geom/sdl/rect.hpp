// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// sdl/rect.hpp:
//	- Specialization of box_traits<> for SDL_Rect

#ifndef BOOST_GEOM_SDL_RECT_HPP
#define BOOST_GEOM_SDL_RECT_HPP

// Boost.Geom
#include <boost/geom/box_traits.hpp>
#include <boost/geom/box_xywh_traits.hpp>

// SDL
#include <SDL.h>

namespace boost { namespace geom {

namespace detail {

struct sdl_rect_prop
{
	typedef SDL_Rect& reference_type;
	typedef const SDL_Rect& param_type;
	typedef Sint16 signed_coord;
	typedef Uint16 unsigned_coord;
	
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
		{ reinterpret_cast<signed_coord&>(self.w)=v; }

		static value_type get(param_type self)
		{ return reinterpret_cast<const signed_coord&>(self.w); }

		static value_type& getset(reference_type self)
		{ return reinterpret_cast<signed_coord&>(self.w); }
	};
	
	struct height
	{
		typedef signed_coord    value_type;
		typedef value_type ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ reinterpret_cast<signed_coord&>(self.h)=v; }

		static value_type get(param_type self)
		{ return reinterpret_cast<const signed_coord&>(self.h); }

		static value_type& getset(reference_type self)
		{ return reinterpret_cast<signed_coord&>(self.h); }
	};
}; // struct sdl_rect_prop
 
} // namespace detail


/// Specializing box_traits to deal with  SDL_Rect
template<>
struct box_traits< SDL_Rect >:
	public box_xywh_traits<SDL_Rect,
			       detail::sdl_rect_prop::x1,
			       detail::sdl_rect_prop::y1,
			       detail::sdl_rect_prop::width,
			       detail::sdl_rect_prop::height>
			       
{
	static const bool is_specialized=true;
};


} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_SDL_RECT_HPP
