/*
 * array_fixed.h
 *
 *  Created on: 28 mar 2013
 *      Author: Johan
 */

#ifndef MPTL_ARRAY_FIXED_H_
#define MPTL_ARRAY_FIXED_H_

#include "../utilities/special_functions.h"
#include "point.h"

namespace mptl{

template< typename T , generic_size_type size, generic_size_type... sizes>
class fixedarray_reference;



template< typename T , generic_size_type... sizes>
class fixedarray{
	typedef generic_size_type size_type;
	typedef T elem_type;

	template < typename... Args >
	constexpr fixedarray(): data(){}
	~fixedarray(){}

	template <typename... Args>
	T& operator()( Args... rest ){ return data[ calc_index(rest...) ]; }

	template <typename... Args>
	constexpr size_type calc_index( size_type index, Args... rest){
		return index * stride(0) + calc_index( rest... );
	}


	fixedarray_reference< elem_type, sizes... >& operator[]( int index ){ return data[ index * stride( 0 ) ]; }
	const fixedarray_reference< elem_type, sizes... >& operator[]( int index ) const { return data[ index * stride( 0 ) ]; }

	constexpr size_type stride( const size_type dim ) const{
		return dim > 1 ? size( dim )*stride( dim - 1 ) : 1 ;
	}

	constexpr size_type size(const size_type dim) const{ return dimension_sizes( dim ); }

	template <size_type dim>
	size_type size() const{ return size(dim); }
private:
	static constexpr point<generic_size_type, sizeof...(sizes)> dimension_sizes = {sizes...};
	data_holder<T> data;
};




template< typename T, generic_size_type dim_size>
class fixedarray<T,dim_size>{
	typedef generic_size_type size_type;

	template < typename... Args >
	constexpr fixedarray( ) : data(dim_size){}
	~fixedarray(){}

	T& operator[]( int index ){ return data[ index ]; }
	const T& operator[]( int index ) const { return data[ index ]; }
	size_type size() const{ return data.initialized_size(); }
private:
	data_holder<T> data;
};





template< typename T , generic_size_type size, generic_size_type... sizes>
class fixedarray_reference{
	typedef generic_size_type size_type;
	typedef T elem_type;

	template < typename... Args >
	constexpr fixedarray_reference( T* in_data ) : data( in_data ){}
	~fixedarray_reference(){}

	fixedarray_reference< elem_type, sizes... >& operator[]( int index ){ return fixedarray_reference<elem_type, sizes...>(); }
	const fixedarray_reference< elem_type, sizes... >& operator[]( int index ) const { return fixedarray_reference<elem_type, sizes...>(); }
private:
	static constexpr point<generic_size_type, sizeof...(sizes)-1> strides = {sizes...};
	T* data;
};


template< typename T, generic_size_type dim_size>
class fixedarray_reference<T,dim_size>{
	typedef generic_size_type size_type;
	typedef T elem_type;

	template < typename... Args >
	constexpr fixedarray_reference( T* in_data ) : data( dim_size ){}
	~fixedarray_reference(){}

	elem_type& operator[]( int index ){ return data[ index ]; }
	const elem_type& operator[]( int index ) const { return data[ index ]; }
	size_type size() const{ return data.initialized_size(); }
private:
	data_holder<T> data;
};

} // namespace_mptl

#endif /* MPTL_ARRAY_FIXED_H_ */
