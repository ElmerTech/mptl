/*
 * array_min_max.h
 *
 *  Created on: 27 apr 2014
 *      Author: johan
 */

#ifndef ARRAY_MIN_MAX_H_
#define ARRAY_MIN_MAX_H_

#include "../mptl_config.h"
#include "../container/range.h"

namespace mptl{

template<class T> inline T array_max(const T* const & array, const int size){
	T current_max = array[0];
	for(int i = 1; i< size; i++){
		if(current_max < array[i])
			current_max = array[i];
	}
	return current_max;
}


template<class T> inline T array_min(const T* const & array, const int size){
	T current_min = array[0];
	for(int i = 1; i< size; i++){
		if(array[i] < current_min)
			current_min = array[i];
	}
	return current_min;
}

template < typename  T, typename  RT = T >
inline range<RT> array_range( const T* const & array, const int size ){
	RT current_max = array[0];
	RT current_min = array[0];
	for(int i = 1; i< size; i++){
		if(array[i] < current_min){
			current_min = array[i];
		}else if(current_max < array[i]){
			current_max = array[i];
		}
	}
	return { current_min, current_max};
}


}



#endif /* ARRAY_MIN_MAX_H_ */
