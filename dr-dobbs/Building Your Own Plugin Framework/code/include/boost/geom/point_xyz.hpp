// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// point_xyz.hpp:
// 	- Implementation type point_xyz<>
//	- Specialization of point_traits<> for class point_xyz<>

#ifndef BOOST_GEOM_POINT_XYZ_HPP
#define BOOST_GEOM_POINT_XYZ_HPP

#include <boost/geom/point_xy.hpp>
#include <boost/geom/point_xyz_traits.hpp>

namespace boost { namespace geom {

// Point implementation: holding the x, y and z properties of a point.
// Value: Type in which X and Y properties will be stored.
// Float: Float type (for floating point properties).
/// See detail/to_float_type.hpp
template<typename Value, typename Float=typename detail::to_float_type<Value>::type>
struct point_xyz:
		public point_xy<Value, Float>
{
	typedef typename point_xy<Value, Float>::value_type value_type;

	value_type z;
	
	point_xyz()
	{}

	point_xyz(value_type mx, value_type my, value_type mz):
		point_xy<Value, Float>(mx, my), z(mz)
	{}
	
};

namespace detail {

template<typename Value, typename Float>
struct point_xyz_prop:
		public detail::point_xy_prop<Value, Float>
{
    private:
	typedef point_xyz<Value, Float> point_type;

    public:
	struct z
	{
		static const bool direct_access=true;
		typedef Value      value_type;
		typedef value_type ret_type;
		
		static value_type get(const point_type& self)
		{ return self.z; }

		static void set(point_type& self, value_type v)
		{ self.z=v; }

		static value_type& getset(point_type& self)
		{ return self.z; }
	};
};

		
} // namespace detail
// back in namespace boost:: geom

/// Specializing point_traits to deal with point_xyz
/// This will allow other parts of the library
/// (notably wrappers in geom/point.hpp) to treat point_xyz as a ``point''.
template<typename Value, typename Float>
struct point_traits< point_xyz<Value, Float> >:
	public point_xyz_traits<
	point_xyz<Value, Float>,
				typename detail::point_xyz_prop<Value, Float>::x,
				typename detail::point_xyz_prop<Value, Float>::y,
				typename detail::point_xyz_prop<Value, Float>::z,
				Float>
				
{ static const bool is_specialized=true; };

} } // namespace boost::geom

#endif // ifndef BOOST_GEOM_POINT_XYZ_HPP
