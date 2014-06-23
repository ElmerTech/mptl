/*
 * special_functions.h
 *
 *  Created on: 21 mar 2013
 *      Author: Johan
 */

#ifndef MPTL_TYPE_UTILS_H_
#define MPTL_TYPE_UTILS_H_

#include "../mptl_config.h"
#include <utility>

namespace mptl{

template <typename T>
typename std::remove_reference<T>::type& lref(T&& value){
	return static_cast<typename std::remove_reference<T>::type&>(value);
}

template <typename T, bool is_const>
struct conditional_const{
	typedef T type;
};
template <typename T>
struct conditional_const<T,true>{
	typedef const T type;
};


}

#endif /* MPTL_TYPE_UTILS_H_ */
