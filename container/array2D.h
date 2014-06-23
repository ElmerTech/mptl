/*
 * array2D.h
 *
 *  Created on: 28 mar 2013
 *      Author: Johan
 */

#ifndef MPTL_ARRAY2D_H_
#define MPTL_ARRAY2D_H_

#include "../mptl_config.h"

namespace mptl{

template<class T>
class array2D{
public:
	typedef T elem_type;
	typedef generic_size_type size_type;

	array2D(size_type in_width, size_type in_height, size_type in_dim_stride):
		max_width(in_width),
		max_height(in_height),
		width(in_width),
		height(in_height),
		dimension_stride(in_dim_stride),
		data( in_dim_stride * in_height )
	{}

	virtual ~array2D(){}

	T* operator[](const int row){ return &data[ row * dimension_stride];}
	const T* operator[](const int row) const { return &data[ row * dimension_stride];}
private:
	size_type max_width, max_height;
	size_type width, height;
	size_type dimension_stride;
	data_holder<elem_type> data;
};



template<class T>
class array2D_reference{
public:
	typedef T elem_type;
	typedef generic_size_type size_type;

	elem_type* operator[](const int row){return point + row * dimension_stride;}
	const elem_type* operator[](const int row) const {return point + row * dimension_stride;}

	array2D_reference(const array2D_reference& original, int y = 0, int x = 0):
		point(original.point + y*original.dimension_stride + x),
		dimension_stride(original.dimension_stride){}

	array2D_reference(elem_type* const & p, size_type in_dim_stride):point(p),dimension_stride(in_dim_stride){}

	array2D_reference( array2D<elem_type>& im, int y, int x )
	:point(im[y] + x),
	 dimension_stride( im.width_step )
	{}

	array2D_reference operator()(const int y, const int x) { return array2D_reference(*this, y , x);}
	const array2D_reference operator()(const int y, const int x) const { return array2D_reference(*this, y , x);}
	array2D_reference& operator=(const array2D_reference&);

private:
	elem_type* point;
	const int dimension_stride;

};

}

#endif /* MPTL_ARRAY2D_H_ */
