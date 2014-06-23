/*
 * special_functions.h
 *
 *  Created on: 21 mar 2013
 *      Author: Johan
 */

#ifndef MPTL_SPECIAL_FUNCTIONS_H_
#define MPTL_SPECIAL_FUNCTIONS_H_

#include "../mptl_config.h"
#include <cstdlib>
#include <cmath>

namespace mptl{

template< generic_size_type size, generic_size_type... sizes >
constexpr generic_size_type product(){
	return size*product<sizes...>();
}

inline double euclidean_remainder(const double numerator, const double denominator){
	const double remainder = std::fmod(numerator, denominator);
	return remainder >= 0 ? remainder : remainder + denominator;
}

template <typename T>
struct div_res{
	long quotient;
	T remainder;
};

template < typename T, typename U >
inline div_res<T> euclid_division(const T numer, const U denom){
	const long quotient = floor( numer / denom );
	const auto prod = quotient * static_cast<T>(denom);
	div_res<T> result{ quotient, numer - prod };
//	if( result.remainder < 0 ){
//		result.quotient--;
//		result.remainder += denom;
//	}
	return result;
}


inline div_t euclid_div(const int numer, const int denom){
	div_t result = std::div( numer, denom );
	while( result.rem < 0 ){
		result.quot--;
		result.rem += denom;
	}
	return result;
}


template < typename T >
inline constexpr T square( T value){
	return value * value;
}

template < typename T >
inline constexpr T quad( T value){
	return square<T>( square<T>( value ));
}

template< typename T >
inline T ramp( const T& value){ return value > 0 ? value : 0; }

template <typename... VTs>
struct relations_struct;
template <typename T, typename... VTs>
struct relations_struct< T,VTs... >{
	static auto min( T value, VTs... more_values )->decltype(value + relations_struct<VTs...>::min(more_values...)){
		return  value < relations_struct<VTs...>::min( more_values... ) ?
				value : relations_struct<VTs...>::min( more_values... );
	}
	static auto max( T value, VTs... more_values )->decltype(value + relations_struct<VTs...>::max(more_values...)){
		return  value > relations_struct<VTs...>::max( more_values... ) ?
				value : relations_struct<VTs...>::max( more_values... );
	}
};
template <typename T>
struct relations_struct< T >{
	static T min( T value ){ return value ;	}
	static T max( T value ){ return value ;	}
};

template < typename T, typename... VTs >
auto min( T value, VTs... more_values )-> decltype( relations_struct<T, VTs...>::min(value, more_values...) ) {
	return relations_struct<T, VTs...>::min( value, more_values... );
}
template < typename T, typename... VTs >
auto max( T value, VTs... more_values )-> decltype( relations_struct<T, VTs...>::max(value, more_values...) ) {
	return relations_struct<T, VTs...>::max( value, more_values... );
}

/**
 * Normally the index should be the euclidean_remainder of index and buffer_length.
 * This function only subtracts buffer_length if index larger than buffer_length.
 */
inline generic_size_type unsafe_local_growing_circular_index(int index, generic_size_type buffer_length){
	return index - ( index - buffer_length < 0 ? 0 : buffer_length );
}


template < size_t pwr, typename T > inline constexpr
typename std::enable_if<std::is_integral<T>::value,T>::type ceil_binary_power(const T value){
	return ( ( ( value - 1 ) >> pwr) + 1 ) << pwr;
}

}


#endif /* MPTL_SPECIAL_FUNCTIONS_H_ */
