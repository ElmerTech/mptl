/*
 * arrays.h
 *
 *  Created on: 21 mar 2013
 *      Author: Johan
 */

#ifndef _MPTL_ARRAYS_H_
#define _MPTL_ARRAYS_H_

#include "../mptl_config.h"
#include "../utilities/special_functions.h"
#include "data_holder.h"

#include "array2D.h"
#include "array_fixed.h"

namespace mptl{


template< typename T , generic_size_type xD>
class array{
	typedef generic_size_type size_type;

	template < typename... Args >
	constexpr array( Args... size ) : data(product<size...>()){}
	~array(){}

	T& operator[]( int index ){ return data[ index ]; }
	const T& operator[]( int index ) const { return data[ index ]; }
	size_type size() const{ return data.size(); }

private:
	point<generic_size_type, xD-1> strides;
	data_holder<T> data;
};



}
#endif /* _MPTL_ARRAYS_H_ */
