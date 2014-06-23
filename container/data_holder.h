/*
 * data_holder.h
 *
 *  Created on: 24 mar 2013
 *      Author: Johan
 */

#ifndef DATA_HOLDER_H_
#define DATA_HOLDER_H_

#include "../mptl_config.h"
#include "range.h"

#include <utility>
#include <type_traits>
#include <iostream>
#include <new>

namespace mptl{

template < typename T >
class linear_init_map{
public:
	typedef generic_size_type size_type;
	typedef size_type index_type;
	typedef T elem_type;

	linear_init_map(): init_size(0){}

	size_type size() const { return init_size; }
	elem_type& get_data( elem_type* data_field, index_type index ){
		return *( data_field + index );
	}
	const elem_type& get_data( const elem_type* data_field, index_type index ) {
		return *( data_field + index );
	}

private:
	size_type init_size;
};


template < typename T, typename init_map = linear_init_map<T> >
class data_chunk{
public:
	typedef generic_size_type size_type;
	typedef T elem_type;

	data_chunk( ):
		data_size(0),
		raw_data( allocate_data_impl(data_size) ),
		initialized(){}

	data_chunk( size_type allocation_size ):
		data_size(allocation_size),
		raw_data( allocate_data_impl(data_size) ),
		initialized(){ }

	~data_chunk( ){
		if( raw_data != nullptr ) {
			deallocate_data_impl();
		}
	}



	data_chunk( const data_chunk& original ):
		data_size( original.data_size ),
		raw_data( allocate_data_impl( data_size ) ),
		initialized( )
	{
		data_copy_impl( original );
	}

	data_chunk& operator=( const data_chunk& original ){
		if( this != &original ){
			if( raw_data != nullptr ){
				if( data_size < original.data_size ){
					deallocate_data_impl();
					data_size = original.data_size;
					raw_data = allocate_data_impl(data_size);
				}
			}else{
				data_size = original.data_size;
				raw_data = allocate_data_impl(data_size);
			}
			data_copy_impl( original );
		}
		return *this;
	}

	data_chunk( data_chunk&& original ):
		data_size( original.data_size ),
		raw_data( original.raw_data ),
		initialized()
	{
		std::swap(initialized, original.initialized);
		original.raw_data = nullptr;
	}

	data_chunk& operator=( data_chunk&& original ){
		std::swap( data_size , original.data_size );
		std::swap( raw_data, original.raw_data );
		std::swap( initialized, original.initialized );
		return *this;
	}


	size_type size() const { return initialized.size(); }
	size_type capacity() const  { return data_size; }
	elem_type& operator[]( const typename init_map::index_type index ){ return data( index ); }
	const elem_type& operator[]( const typename init_map::index_type index ) const { return const_data( index ); }


private:
	elem_type& data( const typename init_map::index_type index ){
		return initialized.get_data( static_cast<elem_type*>(raw_data), index );
	}
	const elem_type& const_data( const typename init_map::index_type index ) const {
		return initialized.get_data( static_cast< elem_type* >(raw_data), index );
	}
	void deallocate_data_impl(){
		operator delete( raw_data );
		raw_data = nullptr;
	}

	static void* allocate_data_impl( size_type data_size){
		constexpr size_type extra_space = alignof( elem_type );
		return operator new( sizeof( elem_type[data_size]  ) + extra_space );
	}
	void data_copy_impl(const data_chunk& original){
		initialized.copy( raw_data, original );
	}

	size_type data_size;
	void* raw_data;
	init_map initialized;
};



template < typename T >
class data_holder{
public:
	typedef generic_size_type size_type;
	typedef T elem_type;

	size_type _data_size;
	elem_type* _data;

	data_holder( ) :
		_data_size(0),
		_data(nullptr){}

	data_holder( size_type allocation_size ):
		_data_size(allocation_size),
		_data( allocate_data_impl(_data_size) ) { }

	~data_holder( ){
		if( _data != nullptr ) {
			deallocate_data_impl();
		}
	}



	data_holder( const data_holder& original ):
		_data_size( original._data_size ),
		_data( allocate_data_impl(_data_size) )
	{
#ifdef MPTL_DATA_HOLDER_DEBUG
		std::cerr << "copying constructor data_holder\n";
		std::cerr.flush();
#endif
		data_copy_impl( original );
	}

	data_holder& operator=( const data_holder& original ){
#ifdef MPTL_DATA_HOLDER_DEBUG
		std::cerr << "copying operator data_holder\n";
		std::cerr.flush();
#endif
		if( this != &original ){
			validate_size_impl( original._data_size );
			data_copy_impl( original );
		}
		return *this;
	}

	data_holder( data_holder&& original ):
		_data_size( original._data_size ),
		_data( original._data )
	{
#ifdef MPTL_DATA_HOLDER_DEBUG
		std::cerr << "moving constructor data_holder\n";
		std::cerr.flush();
#endif
		original._data = nullptr;
		original._data_size = 0;
	}

	data_holder& operator=( data_holder&& original ) {
#ifdef MPTL_DATA_HOLDER_DEBUG
		std::cerr << "moving operator data_holder\n";
		std::cerr.flush();
#endif
		std::swap( _data_size , original._data_size );
		std::swap( _data, original._data );
		return *this;
	}

	range< const elem_type*, range_types::normal> storage_range() const {
		return { start(), end() };
	}

	elem_type* start() { return _data; }
	const elem_type* start() const { return _data; }
	elem_type* end() { return _data + allocated_size_impl(); }
	const elem_type* end() const { return _data + allocated_size_impl(); }

	elem_type* data() { return _data; }
	const elem_type* data() const { return _data; }

	size_type size() const { return allocated_size_impl(); }
	size_type capacity() const  { return allocated_size_impl(); }
	void reserve( size_type new_size ){
		validate_size_impl( new_size );
		std::cerr << _data << " - " << _data_size << "\n";
	}


	elem_type& operator[]( const size_type index ){ return _data[index]; }
	const elem_type& operator[]( const size_type index ) const { return _data[index]; }


private:
	void validate_size_impl( const size_type new_data_size ){
		if(_data != nullptr ){
			if( _data_size < new_data_size ){
				deallocate_data_impl();
				_data_size = new_data_size;
				_data = allocate_data_impl( _data_size );
			}
		}else{
			_data_size = new_data_size;
			_data = allocate_data_impl(_data_size);
		}
	}
	void deallocate_data_impl(){
		delete[] _data;
		_data = nullptr;
	}
	size_type allocated_size_impl() const {
		return _data_size;
	}
	static elem_type* allocate_data_impl( const size_type data_size){
		return new elem_type[data_size];
	}
	void data_copy_impl(const data_holder& original){
		for(size_type i = 0; i < original._data_size; i++ ){
			_data[ i ] = original._data[ i ];
		}
	}
};


template < typename T >
class no_copy_data_holder: public data_holder<T>{
public:
	using typename data_holder<T>::size_type;
	no_copy_data_holder( ) : data_holder<T>() {}
	no_copy_data_holder( size_type allocation_size ) : data_holder<T>( allocation_size ) {}
	no_copy_data_holder( const no_copy_data_holder& ) = delete;
	no_copy_data_holder& operator=( const no_copy_data_holder& ) = delete;
	no_copy_data_holder( no_copy_data_holder&& ) = default;
	no_copy_data_holder& operator=( no_copy_data_holder&& ) = default;
};

//template < typename T >
//class data_holder_reference{
//public:
//	typedef generic_size_type size_type;
//	typedef T elem_type;
//
//	data_holder_reference( ):
//		holder(new data_holder< elem_type >( ) ){}
//
//	data_holder_reference( size_type allocation_size ):
//		holder( new data_holder< elem_type >( allocation_size ) ) { }
//
//	~data_holder_reference( ){
//		if( holder != nullptr ) {
//			delete holder;
//		}
//	}
//
//
//
//	data_holder_reference( const data_holder_reference& original ) = delete;
//	data_holder_reference& operator=( const data_holder_reference& original ) = delete;
//
//	data_holder_reference( data_holder_reference&& original ):
//		holder( original.holder )
//	{
//		std::cerr << "moving constructor data_holder\n";
//		std::cerr.flush();
//		original.holder = nullptr;
//	}
//
//	data_holder_reference& operator=( data_holder_reference&& original ) {
//		std::cerr << "moving operator data_holder\n";
//		std::cerr.flush();
//		std::swap( holder , original.holder );
//		return *this;
//	}
//
//	elem_type* data() { return holder->data(); }
//	const elem_type* data() const { return holder->data(); }
//
//	size_type size() const { return holder->size(); }
//	size_type capacity() const  { return holder->capacity(); }
//	elem_type& operator[]( const size_type index ){ return holder->operator[](index); }
//	const elem_type& operator[]( const size_type index ) const { return holder->operator[](index); }
//private:
//	data_holder< elem_type >* holder;
//};
//



}
#endif /* DATA_HOLDER_H_ */
