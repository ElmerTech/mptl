/*
 * data_space.h
 *
 *  Created on: 24 mar 2013
 *      Author: Johan
 */

#ifndef DATA_ALLOCATOR_H_
#define DATA_ALLOCATOR_H_

#include "../mptl_config.h"

namespace{
#include <new>
}

namespace mptl{

class _no_copy_for_now{
	_no_copy_for_now(const _no_copy_for_now& ) = delete;
	_no_copy_for_now& operator=(const _no_copy_for_now& ) = delete;
};

//template < typename T >
//T* allocate( generic_size_type size ){
//	generic_size_type raw_size = sizeof(T[size]);
//	void* allocated_data = operator new ( raw_size );
//	if( (static_cast<generic_size_type>(allocated_data) & alignof(T)) == 0 )
//		return allocated_data;
//	operator delete( allocated_data);
//}
//
//template < typename T >
//void deallocate( T*& data ){
//	operator delete ( data );
//	data = nullptr;
//}


}

#endif /* DATA_ALLOCATOR_H_ */
