/*
 * mptl_config.h
 *
 *  Created on: 24 mar 2013
 *      Author: Johan
 */

#ifndef MPTL_CONFIG_H_
#define MPTL_CONFIG_H_

#include <iostream>

namespace mptl{

typedef unsigned int generic_size_type;

struct index_exception{
	const int index;
};

inline void mptl_assert( bool test ){
	if(!test){
		std::cerr << "MPTL ASSERT_FAILED!\n";
		std::cerr.flush();
		throw nullptr;
	}
}


}


#endif /* MPTL_CONFIG_H_ */
