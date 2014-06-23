/*
 * range.h
 *
 *  Created on: 24 apr 2013
 *      Author: Johan
 */

#ifndef MPTL_RANGE_H_
#define MPTL_RANGE_H_

#include "../mptl_config.h"
#include "../utilities/special_functions.h"

#include <iostream>

namespace mptl {

enum class range_types: unsigned char { normal, lower_unbound, upper_unbound, all, none };

template < typename T, range_types range_type = range_types::normal >
class range ;

template < typename T >
class range< T, range_types::normal >{
public:
	constexpr range( const T& _min, const T& _max ): _min(_min), _max(_max) {}
	constexpr range( const range& org ): _min(org._min), _max(org._max) {}
	range& operator= ( const range& org ){ _min = org._min; _max = org._max; return *this; }
	range( const range&& org ): _min(org._min), _max(org._max){}
	range& operator= ( const range&& org ){ _min = org._min; _max = org._max; return *this; }

	constexpr bool satisfies_lower( const T& value ) const { return _min <= value; }
	constexpr bool satisfies_upper( const T& value ) const { return value <= _max; }
	constexpr bool satisfies_open_lower( const T& value ) const { return _min < value; }
	constexpr bool satisfies_open_upper( const T& value ) const { return value < _max; }

	constexpr bool in_closed( const T& value ) const { return satisfies_lower(value) && satisfies_upper(value); }
	constexpr bool in_open( const T& value ) const { return satisfies_open_lower(value) && satisfies_open_upper(value); }
	constexpr bool in_open_closed( const T& value ) const { return satisfies_open_lower(value) && satisfies_upper(value); }
	constexpr bool in_closed_open( const T& value ) const { return satisfies_lower(value) && satisfies_open_upper(value); }

	constexpr T lower_bound() const { return _min; }
	constexpr T upper_bound() const { return _max; }
	constexpr T min() const { return _min; }
	constexpr T max() const { return _max; }
	std::ostream& print( std::ostream& os ) const {
		return ( os << "[ " << _min << ", " << _max << " ]" );
	}
private:
	T _min;
	T _max;
};


template < typename T>
class range< T, range_types::lower_unbound >{
public:
	constexpr range( const T& _max ): max(_max) {}
	constexpr range( const range& org ): max(org.max) {}
	range& operator= ( const range& org ){ max = org.max; return *this; };
	range( const range&& org ): max(org.max) {}
	range& operator= ( const range&& org ){ max = org.max; return *this; };

	constexpr bool satisfies_lower( const T& value ) const { return true; }
	constexpr bool satisfies_upper( const T& value ) const {  return value <= max; }
	constexpr bool satisfies_open_lower( const T& value ) const { return true; }
	constexpr bool satisfies_open_upper( const T& value ) const { return value < max; }

	constexpr bool in_closed( const T& value ) const { return satisfies_lower(value) && satisfies_upper(value); }
	constexpr bool in_open( const T& value ) const { return satisfies_open_lower(value) && satisfies_open_upper(value); }
	constexpr bool in_open_closed( const T& value ) const { return satisfies_open_lower(value) && satisfies_upper(value); }
	constexpr bool in_closed_open( const T& value ) const { return satisfies_lower(value) && satisfies_open_upper(value); }

	std::ostream& print( std::ostream& os ) const {
		return ( os << "( -inf, "<< max << " ]" );
	}
private:
	T max;
};

template < typename T>
class range< T, range_types::upper_unbound >{
public:
	constexpr range( const T& _min): min(_min) {}
	constexpr range( const range& org ): min(org.min) {}
	range& operator= ( const range& org ){ min = org.min; return *this; };
	range( const range&& org ): min( org.min ) {}
	range& operator= ( const range&& org ){ min = org.min; return *this; };

	constexpr bool satisfies_lower( const T& value ) const { return min <= value; }
	constexpr bool satisfies_upper( const T& value ) const { return true; }
	constexpr bool satisfies_open_lower( const T& value ) const {return min < value; }
	constexpr bool satisfies_open_upper( const T& value ) const { return true; }

	constexpr bool in_closed( const T& value ) const { return satisfies_lower(value) && satisfies_upper(value); }
	constexpr bool in_open( const T& value ) const { return satisfies_open_lower(value) && satisfies_open_upper(value); }
	constexpr bool in_open_closed( const T& value ) const { return satisfies_open_lower(value) && satisfies_upper(value); }
	constexpr bool in_closed_open( const T& value ) const { return satisfies_lower(value) && satisfies_open_upper(value); }

	std::ostream& print( std::ostream& os ) const {
		return ( os << "[ "<< min << ", inf )" );
	}
private:
	T min;
};


template < typename T>
class range< T, range_types::all >{
public:
	range(){}
	constexpr bool satisfies_lower( const T& value ) const { return true; }
	constexpr bool satisfies_upper( const T& value ) const { return true; }
	constexpr bool satisfies_open_lower( const T& value ) const { return true; }
	constexpr bool satisfies_open_upper( const T& value ) const { return true; }

	constexpr bool in_closed( const T& value ) const { return satisfies_lower(value) && satisfies_upper(value); }
	constexpr bool in_open( const T& value ) const { return satisfies_open_lower(value) && satisfies_open_upper(value); }
	constexpr bool in_open_closed( const T& value ) const { return satisfies_open_lower(value) && satisfies_upper(value); }
	constexpr bool in_closed_open( const T& value ) const { return satisfies_lower(value) && satisfies_open_upper(value); }

	std::ostream& print( std::ostream& os ) const {
		return ( os << "( -inf, inf )" );
	}
};
template < typename T>
class range< T, range_types::none >{
public:
	range(){}
	constexpr bool satisfies_lower( const T& value ) const { return false; }
	constexpr bool satisfies_upper( const T& value ) const { return false; }
	constexpr bool satisfies_open_lower( const T& value ) const { return false; }
	constexpr bool satisfies_open_upper( const T& value ) const { return false; }

	constexpr bool in_closed( const T& value ) const { return satisfies_lower(value) && satisfies_upper(value); }
	constexpr bool in_open( const T& value ) const { return satisfies_open_lower(value) && satisfies_open_upper(value); }
	constexpr bool in_open_closed( const T& value ) const { return satisfies_open_lower(value) && satisfies_upper(value); }
	constexpr bool in_closed_open( const T& value ) const { return satisfies_lower(value) && satisfies_open_upper(value); }

	std::ostream& print( std::ostream& os ) const {
		return ( os << "{}" );
	}
};


template <typename T, range_types range_type>
std::ostream& operator << ( std::ostream& os, const range<T,range_type>& range_value ){
	return range_value.print( os );
}

/***
 * Combines the result of multiple ranges
 */
template <typename T1, typename T2, range_types range_type1, range_types range_type2 >
struct range_combine_struct{
	typedef double range_value_type;
	typedef range<range_value_type, range_types::normal> return_type;
	static inline return_type combine( const range<T1,range_type1> a, const range<T2,range_type2>& b ){
		return return_type( min(a.min(),b.min()),max(a.max(),b.max()));
	}
};

template < typename T1, typename T2 >
struct range_combine_struct<T1,T2,range_types::upper_unbound, range_types::lower_unbound>{
	static constexpr range_types range_type1 = range_types::upper_unbound;
	static constexpr range_types range_type2 = range_types::lower_unbound;

	typedef double range_value_type;
	typedef range<range_value_type, range_types::all> return_type;
	static inline return_type combine( const range<T1,range_types::upper_unbound> a, const range<T2,range_types::lower_unbound>& b ){
		return return_type();
	}
};
template < typename T1, typename T2 >
struct range_combine_struct<T1,T2,range_types::lower_unbound, range_types::upper_unbound>{
	static constexpr range_types range_type1 = range_types::lower_unbound;
	static constexpr range_types range_type2 = range_types::upper_unbound;

	typedef double range_value_type;
	typedef range<range_value_type, range_types::all> return_type;
	static inline return_type combine( const range<T1,range_type2> a, const range<T2,range_type2>& b ){
		return return_type();
	}
};

template < typename T1, typename T2, range_types range_type1 >
struct range_combine_struct<T1,T2,range_type1, range_types::all>{
	static constexpr range_types range_type2 = range_types::all;
	typedef double range_value_type;
	typedef range<range_value_type, range_types::all> return_type;
	static inline return_type combine( const range<T1,range_type1> a, const range<T2,range_type2>& b ){
		return return_type();
	}
};
template < typename T1, typename T2, range_types range_type2 >
struct range_combine_struct<T1,T2, range_types::all, range_type2>{
	static constexpr range_types range_type1 = range_types::all;
	typedef double range_value_type;
	typedef range<range_value_type, range_types::all> return_type;
	static inline return_type combine( const range<T1,range_type1> a, const range<T2,range_type2>& b ){
		return return_type();
	}
};

template < typename T1, typename T2, range_types range_type1 >
struct range_combine_struct<T1,T2,range_type1, range_types::none>{
	static constexpr range_types range_type2 = range_types::none;
	typedef T1 range_value_type;
	typedef range<range_value_type, range_type1> return_type;
	static inline return_type combine( const range<T1,range_type1> a, const range<T2,range_type2>& b ){
		return a;
	}
};
template < typename T1, typename T2, range_types range_type2 >
struct range_combine_struct<T1,T2, range_types::none, range_type2>{
	static constexpr range_types range_type1 = range_types::none;
	typedef T2 range_value_type;
	typedef range<range_value_type, range_type2> return_type;
	static inline return_type combine( const range<T1,range_type1> a, const range<T2,range_type2>& b ){
		return b;
	}
};


/****
 * Convenience functions
 */
template <typename T1, typename T2, range_types range_type1, range_types range_type2 >
inline typename range_combine_struct<T1,T2,range_type1,range_type2>::return_type
combine( const range<T1,range_type1> a, const range<T2,range_type2>& b ){
	return range_combine_struct<T1,T2,range_type1,range_type2>::combine(a,b);
}



template <typename T>
inline bool in_range( const T& x, const T& x_min, const T& x_max ){
	return (x_min <= x) && ( x <= x_max );
}

template <typename T>
inline bool in_range( const T& x, const range<T>& range ){
	return range.satisfies( x );
}


} // namespace_mptl

#endif /* MPTL_RANGE_H_ */
