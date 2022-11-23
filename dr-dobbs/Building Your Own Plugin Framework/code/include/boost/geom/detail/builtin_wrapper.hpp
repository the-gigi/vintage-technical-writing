// Boost.Geom

// Copyright (C) 2005 Anis Benyelloul (benyelloul@maildz.com).

// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/geom for library home page
//------------------------------------------------------------------------------

// detail/builtin_wrapper.hpp:
// 	- Class builtin_wrapper<>
//	- Class getset_builtin<>

#ifndef BOOST_GEOM_DETAIL_BUILTIN_WRAPPER
#define BOOST_GEOM_DETAIL_BUILTIN_WRAPPER

namespace boost { namespace geom { namespace detail {

/// Proxy-Class around a builtin typed object.
///
/// This acts like a builtin but calls a user supplied function when
/// someone tries to modifiy it, and another user supplied function
/// when someone tries to read it(to read its value). 
///
/// This is used to implement property syntax. i.e if you want to have
/// syntax like: point pt; pt.rho()=pt.theta(); and if point isn't
/// actually storing its RHO property as a data field, then pt.rho() must
/// return a proxy that will call set_rho() when bein assigned to.
///
/// Property is supposed to be a property struct as found in traits
/// classes like (point_traits, box_traits) (see
/// geom/point_traits.hpp, geom/point_xy.hpp)
///
/// Host is the type hosting(owning) Property. That is, the first
/// argument to Property::set will be something akin to Host&. And the
/// first (and only) argument to Property::get will look like const Host&.
template<typename Property, typename Host>
struct builtin_wrapper
{
    private:
	typedef typename Property::value_type value_type;

	// Note: ``this'' is (also) a pointer to the host objet itself !!!!!
	Host& host()
	{ return *reinterpret_cast<Host*const>(this);}

	const Host& host() const
	{ return *reinterpret_cast<const Host*>(this);}

	// Disabling unwanted interface
	builtin_wrapper(const builtin_wrapper&);
	void operator&() const;

    public:
	builtin_wrapper(); // Never defined

	operator value_type () const
	{ return Property::get(host()); }

	builtin_wrapper& operator= (value_type v)
	{ Property::set(host(), v); return *this; }

	builtin_wrapper& operator+= (value_type v)
	{ Property::set(host(), Property::get(host())+v); return *this;}

	builtin_wrapper& operator-= (value_type v)
	{ Property::set(host(), Property::get(host())-v); return *this;}

	builtin_wrapper& operator*= (value_type v)
	{ Property::set(host(), Property::get(host())*v); return *this;}

	builtin_wrapper& operator/= (value_type v)
	{ Property::set(host(), Property::get(host())/v); return *this;}

	builtin_wrapper& operator%= (value_type v)
	{ Property::set(host(), Property::get(host())%v); return *this;}

	builtin_wrapper& operator&= (value_type v)
	{ Property::set(host(), Property::get(host())&v); return *this;}

	builtin_wrapper& operator|= (value_type v)
	{ Property::set(host(), Property::get(host())|v); return *this;}

	builtin_wrapper& operator^= (value_type v)
	{ Property::set(host(), Property::get(host())^v); return *this;}

	builtin_wrapper& operator>>= (value_type v)
	{ Property::set(host(), Property::get(host())>>v); return *this;}

	builtin_wrapper& operator<<= (value_type v)
	{ Property::set(host(), Property::get(host())<<v); return *this;}

	builtin_wrapper& operator++ ()
	{ Property::set(host(), Property::get(host())+1); return *this;}

	builtin_wrapper& operator-- ()
	{ Property::set(host(), Property::get(host())-1); return *this;}

	value_type operator++ (int)
	{
		value_type ret=Property::get(host());
		Property::set(host(), ret+1);
		return ret;
	}

	value_type operator-- (int)
	{
		value_type ret=Property::get(host());
		Property::set(host(), ret-1);
		return ret;
	}

};

/// Template struct that simplifies builtin_wrapper usage
///
/// This is because sometimes you can return a reference to the
/// property, but sometimes you can't, and you must return a proxy
/// object. This little struct make it easier to choose..
///
template<typename Property, typename Host, bool direct_access=Property::direct_access>
struct getset_builtin
{
	typedef builtin_wrapper<Property, Host>& type;

	// Note how we create a builtin_wrapper from a host object. A
	// (zero-cost) reinterpret_cast and a return by ref, nothing
	// need to be copied or constructed. (see builtin_wrapper's
	// body for how this fits into the rest..).
	static
	type perform(Host& h)
	{ return reinterpret_cast<type>(h);}
};

template<typename Property, typename Host>
struct getset_builtin<Property, Host, true>
{
	typedef typename Property::value_type& type; 

	static
	type perform(Host& h)
	{ return Property::getset(h); }
};

} } } // namespace boost::geom::detail

#endif // #ifndef BOOST_GEOM_DETAIL_BUILTIN_WRAPPER
