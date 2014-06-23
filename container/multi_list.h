/*
 * multi_list.h
 *
 *  Created on: 23 apr 2013
 *      Author: Johan
 */

#ifndef LINKED_MULTI_LIST_H_
#define LINKED_MULTI_LIST_H_

#include "../mptl_config.h"
#include "data_holder.h"
#include <iostream>
#include <vector>

namespace mptl{

template <typename T>
class multi_list{
public:
	typedef generic_size_type size_type;
	typedef T data_type;
	typedef no_copy_data_holder< data_type > data_holder_type;
private:
	struct temp_internal_index {
		size_type list_index;
		size_type data_index;
	};
	temp_internal_index get_internal_index( const size_type index ) const {
		temp_internal_index _index{ 0, index };
		while( _index.data_index >= data_lists[ _index.list_index ].size() ){
			_index.data_index -= data_lists[ _index.list_index ].size();
			++_index.list_index;
		}
//		if( data_lists.size() > 1 ){
//			std::cout << "index: " << index << " => [ " << _index.list_index << ", " << _index.data_index << " ]";
//			std::cout.flush();
//		}
		return _index;
	}
	struct list_element{
		data_type* front;
		data_type* back;
		data_type* end;

		list_element( data_type* const first, data_type* const last ):
			front( first ),
			back( first ),
			end( last )
		{}

		size_type size() const { return back - front; }
		size_type capacity() const { return end - front; }
		bool push_guard() const { return back < end; }
		void push_back( const data_type& value ){ *back = value; ++back; }
		bool pop_guard() const { return back > front; }
		void pop_back( ){ --back; }

		T& operator[] (size_type index){ return front[index]; }
		const T& operator[] (size_type index) const { return front[index]; }
	};

	list_element& create_new_list( const size_type list_size ){
		data_areas.emplace_back( list_size );
		data_holder_type& new_data = data_areas.back();
		data_lists.emplace_back( new_data.data(), new_data.data() + new_data.capacity() );
		return data_lists.back();
	}
	list_element& create_new_list( ){
		return create_new_list( std::max( size() >> 1, 8u ));
	}
	list_element& get_last_data_list(){
		return data_lists.empty() ? create_new_list() : data_lists.back();
	}
	list_element& last_nonempty_list(){
		return data_lists[ last_nonempty_list_index() ];
	}
	size_type last_nonempty_list_index() const {
		size_type list_index = data_lists.size();
		do{
			if( list_index == 0 )
				throw index_exception{ static_cast<int>( list_index ) - 1 };
		} while( !( data_lists[ --list_index ].pop_guard()) );
		return list_index;
	}

public:
	multi_list( ):
		data_areas(),
		data_lists()
	{
	}
	multi_list( const size_type initial_size ):
		data_areas(),
		data_lists()
	{
		create_new_list( initial_size );
	}

	size_type size() const {
		size_type size_sum = 0;
		for( const list_element& list: data_lists )
			size_sum += list.size();
		return size_sum;
	}

	data_type& operator[] ( const size_type index ){
		temp_internal_index internal_index = get_internal_index(index);
		return data_lists[ internal_index.list_index ][ internal_index.data_index ];
	}
	const data_type& operator[] ( const size_type index ) const {
		temp_internal_index internal_index = get_internal_index(index);
		return data_lists[ internal_index.list_index ][ internal_index.data_index ];
	}
	void print_size_info( std::ostream& os ) const {
		os << "[ ";
		for( const list_element& _list : data_lists )
			os << _list.size() << " ";
		os << "]";
	}
	void reserve( size_type new_size ){
		const size_type current_size = size();
		if( new_size > current_size )
			create_new_list( new_size - current_size );
	}
	void push_back( const data_type& value ){
#ifdef MPTL_MULTI_LIST_DEBUG
		std::cout << "pub:";
		print_size_info(cout);
#endif
		list_element& back = get_last_data_list();
		if( back.push_guard() ){
			back.push_back( value );
		}else {
			create_new_list().push_back( value );
		}
#ifdef MPTL_MULTI_LIST_DEBUG

		std::cout << "pua:";
		print_size_info(std::cout);
		std::cout << "\n";
#endif
	}


	void pop_back(){
#ifdef MPTL_MULTI_LIST_DEBUG
		std::cout << "pob:";
		print_size_info(cout);
#endif
		last_nonempty_list().pop_back();
#ifdef MPTL_MULTI_LIST_DEBUG
		std::cout << "poa:";
		print_size_info(std::cout);
		std::cout << "\n";
#endif
	}

private:
	std::vector< data_holder_type > data_areas;
	std::vector< list_element > data_lists;
};

}

#endif /* LINKED_MULTI_LIST_H_ */
