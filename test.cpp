/*
 * test.cpp
 *
 *  Created on: 24 mar 2013
 *      Author: Johan
 */

#include <iostream>
#include "container/arrays.h"
#include "container/point.h"
#include "container/flag_holder.h"
using namespace mptl;

int main(int argc, char** argv){
	typedef int test_type;
	typedef double test_type2;
	const int xD = 5;

	point<test_type,xD> p(1,2,3,4,5);
	point<test_type2,xD> p2(1.3,2.5,3.1,4.2,5.7);
	auto p3 = p+p2;
	p3 += p;

	auto pc = mptl::round<char>(p3);
	pc += 'a';

	p += point<test_type,xD>(10);
	std::cout << p << " : " << sizeof(p) << " =? " << (xD*sizeof(test_type))<<"\n";
	std::cout << p3 << " : " << sizeof(p3) << " =? " << (xD*sizeof(test_type2)) << "\n";
	std::cout << pc << " : " << sizeof(p3) << " =? " << (xD*sizeof(test_type2)) << "\n";
	std::cout << (p3 - round<int>(p3)) << " : " << sizeof(p3) << " =? " << (xD*sizeof(test_type2));
	return 0;
}
