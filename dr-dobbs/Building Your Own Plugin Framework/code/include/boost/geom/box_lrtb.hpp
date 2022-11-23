// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// box_lrtb.hpp:
// 	- Implementation type box_lrtb<>
//	- Specialization of box_traits<> for class box_lrtb<>

#ifndef BOOST_GEOM_BOX_LRTB_HPP
#define BOOST_GEOM_BOX_LRTB_HPP

#include <boost/geom/box_lrtb_traits.hpp>
#include <boost/geom/box_traits.hpp>

namespace boost { namespace geom {

/// Box implementation. Holds its xmin, xmax, ymin and ymax
/// fields of type ``Value''. The z is always zero as well as the
/// depth, i.e. these are flat boxes layed on the z=0 plan.
template<typename Value>
struct box_lrtb
{
	typedef Value value_type;
	value_type xmin, ymin, xmax, ymax;

	box_lrtb()
	{}

	box_lrtb(value_type mxmin, value_type mymin,
		 value_type mxmax, value_type mymax):
		xmin(mxmin), ymin(mymin), xmax(mxmax),
		ymax(mymax)
	{}
	
};

namespace detail {

template<typename Value>
struct box_lrtb_prop
{
	typedef box_lrtb<Value>& reference_type;
	typedef const box_lrtb<Value>& param_type;
	
	struct xmin 
	{
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.xmin=v;}

		static ret_type get(param_type self)
		{ return self.xmin; }

		static value_type& getset(reference_type self)
		{ return self.xmin; }
	};

	struct xmax 
	{
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.xmax=v; }

		static value_type get(param_type self)
		{ return self.xmax; }

		static value_type& getset(reference_type self)
		{ return self.xmax; }
	};

	struct ymin 
	{
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.ymin=v;}

		static ret_type get(param_type self)
		{ return self.ymin; }

		static value_type& getset(reference_type self)
		{ return self.ymin; }
	};

	struct ymax 
	{
		typedef Value        value_type;
		typedef value_type   ret_type;

		static const bool direct_access=true;
		
		static void set(reference_type self, value_type v)
		{ self.ymax=v; }

		static value_type get(param_type self)
		{ return self.ymax; }

		static value_type& getset(reference_type self)
		{ return self.ymax; }
	};
};

} // namespace detail

// Back in namespace boost { namespace geom

/// Specializing box_traits to deal with  box_lrtb
template<typename Value>
struct box_traits< box_lrtb<Value> >:
	public box_lrtb_traits<box_lrtb<Value>,
			       typename detail::box_lrtb_prop<Value>::xmin,
			       typename detail::box_lrtb_prop<Value>::xmax,
			       typename detail::box_lrtb_prop<Value>::ymin,
			       typename detail::box_lrtb_prop<Value>::ymax>

{ static const bool is_specialized=true; };


} } // namespace boost::geom


#endif // ifndef BOOST_GEOM_BOX_LRTB_HPP
