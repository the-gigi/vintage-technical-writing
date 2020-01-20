// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// qt/point.hpp:
//	- Specialization of point_traits<> for QT class QPoint

#ifndef BOOST_GEOM_QT_POINT_HPP
#define BOOST_GEOM_QT_POINT_HPP

// Qt
#include <qpoint.h>

// Boost.Geom
#include <boost/geom/point_traits.hpp>
#include <boost/geom/point_xy_traits.hpp>
#include <boost/geom/detail/to_float_type.hpp>

namespace boost { namespace geom {

namespace detail {
struct qpoint_prop
{
    private:
	typedef QPoint point_type;

    public:
	struct x 
	{
		static const bool direct_access=true;
		typedef QCOORD      value_type;
		typedef value_type ret_type;
		
		static ret_type get(const point_type& self)
		{ return self.x(); }

		static void set(point_type& self, value_type v)
		{ self.rx()=v; }

		static value_type& getset(point_type& self)
		{ return self.rx(); }
	};

	struct y
	{
		static const bool direct_access=true;
		typedef QCOORD      value_type;
		typedef value_type ret_type;
		
		static ret_type get(const point_type& self)
		{ return self.y(); }

		static void set(point_type& self, value_type v)
		{ self.ry()=v; }

		static value_type& getset(point_type& self)
		{ return self.ry(); }
	};
};

		
} // namespace detail
// back in namespace boost { namespace geom

/// Specializing point_traits to deal with QPoint
///  See geom/point_xy_traits.hpp
template<>
struct point_traits< QPoint >:
	public point_xy_traits<QPoint,
			       detail::qpoint_prop::x,
			       detail::qpoint_prop::y,
			       double>
				
{
    private:
	typedef point_xy_traits<QPoint, detail::qpoint_prop::x,
				detail::qpoint_prop::y, double> base_type;

    public:
	static const bool is_specialized=true;

	using base_type::add;
	void add(QPoint& lhs, const QPoint rhs)
	{ lhs+=rhs; }
	
	using base_type::sub;
	void sub(QPoint& lhs, const QPoint rhs)
	{ lhs-=rhs; }

	using base_type::mul;
	void mul(QPoint& lhs, double v)
	{ lhs*=v; }
	void mul(QPoint& lhs, int v)
	{ lhs*=v; }

	using base_type::div;
	void div(QPoint& lhs, double v)
	{ lhs/=v; }
	void div(QPoint& lhs, int v)
	{ lhs/=v; }

	using base_type::equal;
	bool equal(const QPoint& lhs, const QPoint& rhs)
	{ return lhs==rhs; }
	
	using base_type::assign;
	void assign(QPoint& lhs, const QPoint& rhs)
	{ lhs=rhs; }

	using base_type::neg;
	void neq(QPoint& self)
	{
		self.rx()=-self.rx();
		self.ry()=-self.ry();
	}
	
	
};


} } // namespace boost::geom

#endif // ifndef BOOST_GEOM_QT_POINT_HPP
