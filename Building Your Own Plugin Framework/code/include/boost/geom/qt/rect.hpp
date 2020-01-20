// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// qt/rect.hpp:
//	- Specialization of box_traits<> for class QRect

#ifndef BOOST_GEOM_QT_RECT_HPP
#define BOOST_GEOM_QT_RECT_HPP

// Qt
#include <qrect.h>

#include <boost/geom/box_traits.hpp>
#include <boost/geom/box_lrtb_traits.hpp>


namespace boost { namespace geom {

namespace detail {

struct qrect_prop
{
	typedef QRect box_type;
	
	struct xmin 
	{
		typedef QCOORD       value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(box_type& self, value_type v)
		{ self.rLeft()=v;}

		static ret_type get(const box_type& self)
		{ return self.left(); }

		static value_type& getset(box_type& self)
		{ return self.rLeft(); }
	};

	struct xmax 
	{
		typedef QCOORD        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(box_type& self, value_type v)
		{ self.rRight()=v; }

		static value_type get(const box_type& self)
		{ return self.right(); }

		static value_type& getset(box_type& self)
		{ return self.rRight(); }
	};

	struct ymin 
	{
		typedef QCOORD        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(box_type& self, value_type v)
		{ self.rTop()=v;}

		static ret_type get(const box_type& self)
		{ return self.top(); }

		static value_type& getset(box_type& self)
		{ return self.rTop(); }
	};

	struct ymax 
	{
		typedef QCOORD        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(box_type& self, value_type v)
		{ self.rBottom()=v; }

		static value_type get(const box_type& self)
		{ return self.bottom(); }

		static value_type& getset(box_type& self)
		{ return self.rBottom(); }
	};
};

} // namespace detail

// Back in namespace boost { namespace geom

/// Specializing box_traits to deal with QRect
template<>
struct box_traits< QRect >:
	public box_lrtb_traits<QRect,
			       detail::qrect_prop::xmin,
			       detail::qrect_prop::xmax,
			       detail::qrect_prop::ymin,
			       detail::qrect_prop::ymax>

{ static const bool is_specialized=true; };


} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_QT_RECT_HPP
