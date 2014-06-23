/*
 * circular_buffer.h
 *
 *  Created on: 17 apr 2013
 *      Author: Johan
 */

#ifndef MPTL_CIRCULAR_BUFFER_H_
#define MPTL_CIRCULAR_BUFFER_H_

#include "../mptl_config.h"
#include "data_holder.h"

namespace mptl{

template <typename T>
class circular_buffer{
public:
	typedef generic_size_type size_type;
	typedef generic_size_type pos_type;

	circular_buffer( size_type minimum_size ):
		index_mask(0),
		data( 1 << ( 8 * sizeof(minimum_size) - __builtin_clz( minimum_size )) )
	{
		mptl_assert( __builtin_popcount( data.capacity()) == 1 );
		index_mask = data.capacity() - 1;
	}

	T& operator []( pos_type index){
		return data[index & index_mask];
	}
	const T& operator []( pos_type index) const {
		return data[index & index_mask];
	}
private:
	size_type index_mask;
	data_holder<T> data;
};

}

#endif /* MPTL_CIRCULAR_BUFFER_H_ */
