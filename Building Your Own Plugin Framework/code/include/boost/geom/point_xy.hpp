// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// point_xy.hpp:
// 	- Implementation type point_xy<>
//	- Specialization of point_traits<> for class point_xy<>

#ifndef BOOST_GEOM_POINT_XY_HPP
#define BOOST_GEOM_POINT_XY_HPP

#include <boost/geom/point_traits.hpp>
#include <boost/geom/point_xy_traits.hpp>
#include <boost/geom/detail/to_float_type.hpp>

namespace boost { namespace geom {

// Point implementation: holding the x and y properties of a point.
// Value: Type in which X and Y properties will be stored.
// Float: Float type (for floating point properties).
/// See detail/to_float_type.hpp
template<typename Value, typename Float=typename detail::to_float_type<Value>::type>
struct point_xy
{
	typedef Value value_type;
	typedef Float float_type;
	
	value_type x, y;

	point_xy()
	{}

	point_xy(value_type mx, value_type my): x(mx), y(my)
	{}
	
};

namespace detail {
template<typename Value, typename Float>
struct point_xy_prop
{
    private:
	typedef point_xy<Value, Float> point_type;

    public:
	struct x 
	{
		static const bool direct_access=true;
		typedef Value      value_type;
		typedef value_type ret_type;
		
		static ret_type get(const point_type& self)
		{ return self.x; }

		static void set(point_type& self, value_type v)
		{ self.x=v; }

		static value_type& getset(point_type& self)
		{ return self.x; }
	};
	
	struct y 
	{
		static const bool direct_access=true;
		typedef Value      value_type;
		typedef value_type ret_type;
		
		static ret_type get(const point_type& self)
		{ return self.y; }

		static void set(point_type& self, value_type v)
		{ self.y=v; }

		static value_type& getset(point_type& self)
		{ return self.y; }
	};

};

		
} // namespace detail
// back in namespace boost { namespace geom

/// Specializing point_traits to deal with point_xy
///  See geom/point_xy_traits.hpp
template<typename Value, typename Float>
struct point_traits< point_xy<Value, Float> >:
	public point_xy_traits<point_xy<Value, Float>,
			       typename detail::point_xy_prop<Value, Float>::x,
			       typename detail::point_xy_prop<Value, Float>::y,
			       Float>
				
{
	static const bool is_specialized=true;
};

} } // namespace boost::geom

#endif // ifndef BOOST_GEOM_POINT_XY_HPP
