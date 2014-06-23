/*
 * point.h
 *
 *  Created on: 20 mar 2013
 *      Author: Johan
 */

#ifndef MPTL_POINT_H_
#define MPTL_POINT_H_

#include "../mptl_config.h"
#include <ostream>
#include <initializer_list>

namespace mptl {


namespace __point_data_def{

template< typename T, generic_size_type xD>
struct point_data{
	T dim_value;
	point_data<T,xD-1> rest;

	template<  typename RetT, typename ArgT, RetT S( const ArgT& ) >
	struct initialize_point_data_unary{
		constexpr initialize_point_data_unary( const point_data<ArgT,xD>& _a ):
				a(_a)
		{}
		const point_data<ArgT,xD>& a;
	};
	template<  typename A, typename B, T S(const A&, const B&) >
	struct initialize_point_data{
		constexpr initialize_point_data(const point_data<A,xD>& _a, const point_data<B,xD>& _b):
				a(_a),
				b(_b)
		{}
		const point_data<A,xD>& a;
		const point_data<B,xD>& b;
	};
	template<  typename A, typename B, T S(const A&, const B&) >
	struct initialize_point_scalar_data{
		constexpr initialize_point_scalar_data(const point_data<A,xD>& _a, const B& _b):
				a(_a),
				b(_b)
		{}
		const point_data<A,xD>& a;
		const B& b;
	};

	constexpr point_data(const T& value, bool uniform = false):dim_value(value),rest({uniform?value:T(),uniform}){}

	constexpr point_data():dim_value(),rest(){}

	template< typename... Args >
	constexpr point_data(const T& value, const Args&... _remaining):dim_value(value),rest(_remaining...){}

	template<  typename A, typename B, T S(const A&, const B&) >
	constexpr point_data( const initialize_point_data<A,B,S>& _data ):
		dim_value( S(_data.a.dim_value,_data.b.dim_value)),
		rest( typename point_data<T,xD-1>::template initialize_point_data<A,B,S>{_data.a.rest,_data.b.rest})
	{}
	template<  typename A, typename B, T S(const A&, const B&) >
	constexpr point_data( const initialize_point_scalar_data<A,B,S>& _data ):
		dim_value( S(_data.a.dim_value,_data.b)),
		rest( typename point_data<T,xD-1>::template initialize_point_scalar_data<A,B,S>{ _data.a.rest,_data.b })
	{}
	template<  typename RetT, typename ArgT, RetT S( const ArgT& ) >
	constexpr point_data( const initialize_point_data_unary<RetT,ArgT,S>& _data ):
		dim_value( S(_data.a.dim_value)),
		rest( typename point_data<T,xD-1>::template initialize_point_data_unary<RetT,ArgT,S>{_data.a.rest})
	{}
};



template <typename A, typename B, generic_size_type xD >
inline void operator+=( point_data<A,xD>& a, const point_data<B,xD>& b){
	a.dim_value += b.dim_value;
	a.rest += b.rest;
}
template <typename A, generic_size_type xD >
inline void operator+=( point_data<A,xD>& a, const A& val){
	a.dim_value += val;
	a.rest += val;
}
template <typename A, typename B, generic_size_type xD >
inline void operator-=( point_data<A,xD>& a, const point_data<B,xD>& b){
	a.dim_value -= b.dim_value;
	a.rest -= b.rest;
}
template <typename A, generic_size_type xD >
inline void operator-=( point_data<A,xD>& a, const A& val){
	a.dim_value -= val;
	a.rest -= val;
}


template <typename T, generic_size_type xD >
inline std::ostream& operator<<( std::ostream& os , const point_data<T,xD>& p) {
	return ( os << p.dim_value << ", " << p.rest );
}


template< typename T>
struct point_data<T,1>{
	T dim_value;

	template<  typename RetT, typename ArgT, RetT S( const ArgT& ) >
	struct initialize_point_data_unary{
		constexpr initialize_point_data_unary( const point_data<ArgT,1>& _a ):
			a(_a)
		{}
		const point_data<ArgT,1>& a;
	};
	template<  typename A, typename B, T S(const A&, const B&) >
	struct initialize_point_data{
		constexpr initialize_point_data(const point_data<A,1>& _a, const point_data<B,1>& _b):
			a(_a),
			b(_b)
		{}
		const point_data<A,1>& a;
		const point_data<B,1>& b;
	};
	template<  typename A, typename B, T S(const A&, const B&) >
	struct initialize_point_scalar_data{
		constexpr initialize_point_scalar_data(const point_data<A,1>& _a, const B& _b):
				a(_a),
				b(_b)
		{}
		const point_data<A,1>& a;
		const B& b;
	};

	constexpr point_data(const T& value, bool uniform):dim_value(value){}
	constexpr point_data(const T& value = T()):dim_value(value){}

	template<  typename A, typename B, T S(const A&, const B&) >
	constexpr point_data( const initialize_point_data<A,B,S>& _data ):
		dim_value( S(_data.a.dim_value,_data.b.dim_value))
	{}
	template<  typename A, typename B, T S(const A&, const B&) >
	constexpr point_data( const initialize_point_scalar_data<A,B,S>& _data ):
		dim_value( S(_data.a.dim_value,_data.b))
	{}
	template<  typename RetT, typename ArgT, RetT S( const ArgT& ) >
	constexpr point_data( const initialize_point_data_unary<RetT,ArgT,S>& _data ):
		dim_value( S(_data.a.dim_value))
	{}
};

template <typename A, typename B>
inline void operator+=( point_data<A,1>& a, const point_data<B,1>& b){
	a.dim_value += b.dim_value;
}
template <typename A>
inline void operator+=( point_data<A,1>& a, const A& val){
	a.dim_value += val;
}
template <typename A, typename B>
inline void operator-=( point_data<A,1>& a, const point_data<B,1>& b){
	a.dim_value -= b.dim_value;
}
template <typename A>
inline void operator-=( point_data<A,1>& a, const A& val){
	a.dim_value -= val;
}

template <typename T >
inline std::ostream& operator<<( std::ostream& os , const point_data<T, 1>& p) {
	return ( os << p.dim_value ); }

}




template< typename T, generic_size_type xD>
struct point{
	typedef T data_type;
	typedef __point_data_def::point_data<T,xD> point_data;
	template<  typename RetT, typename ArgT, RetT S( const ArgT& ) >
	using initialize_point_data_unary = typename point_data::template initialize_point_data_unary<RetT,ArgT,S>;
	template < typename A, typename B, T S(const A&, const B&)>
	using initialize_point_data = typename point_data::template initialize_point_data<A,B,S>;
	template < typename A, typename B, T S(const A&, const B&)>
	using initialize_point_scalar_data = typename point_data::template initialize_point_scalar_data<A,B,S>;

	union{
		T coords[xD];
		point_data dim_values;
	};

	constexpr point(const T& value) : dim_values( value, true ){}

	template< typename... Args >
	constexpr point(Args... _coords) : dim_values( _coords... ){}

	template<  typename RetT, typename ArgT, RetT S( const ArgT& ) >
	constexpr point( initialize_point_data_unary<RetT,ArgT,S> data ) : dim_values( data ){}
	template< typename A, typename B, T S(const A&, const B&) >
	constexpr point( initialize_point_data<A,B,S> data ) : dim_values( data ){}
	template< typename A, typename B, T S(const A&, const B&) >
	constexpr point( initialize_point_scalar_data<A,B,S> data ) : dim_values( data ){}

};

template< typename T>
union p2dbase{
	typedef __point_data_def::point_data<T,2> point_data;
	T coords[2];
	point_data dim_values;
	struct{
		T x,y;
	};
};

template< typename T>
struct point<T,2>{
	typedef T data_type;
	typedef __point_data_def::point_data<T,2> point_data;
	template<  typename RetT, typename ArgT, RetT S( const ArgT& ) >
	using initialize_point_data_unary = typename point_data::template initialize_point_data_unary<RetT,ArgT,S>;
	template < typename A, typename B, T S(const A&, const B&)>
	using initialize_point_data = typename point_data::template initialize_point_data<A,B,S>;
	template < typename A, typename B, T S(const A&, const B&)>
	using initialize_point_scalar_data = typename point_data::template initialize_point_scalar_data<A,B,S>;

	union{
		T coords[2];
		point_data dim_values;
		struct{
			T x,y;
		};
	};
	constexpr point( const T& value ): dim_values( value, value ){}
	constexpr point( const T& _x, const T& _y ):x(_x),y(_y){}

	template<  typename RetT, typename ArgT, RetT S( const ArgT& ) >
	constexpr point( initialize_point_data_unary<RetT, ArgT,S> data ) : dim_values( data ){}
	template< typename A, typename B, T S(const A&, const B&) >
	constexpr point( initialize_point_data<A,B,S> data ) : dim_values( data ){}
	template< typename A, typename B, T S(const A&, const B&) >
	constexpr point( initialize_point_scalar_data<A,B,S> data ) : dim_values( data ){}
};

template <typename RetT,typename  ArgT>
inline RetT do_round( const ArgT& _value ){ return std::round(_value ); }
template <typename A, typename B>
inline auto plus( const A& _a, const B& _b)->decltype(_a+_b){ return _a+_b; }
template <typename A, typename B>
inline auto minus( const A& _a, const B& _b)->decltype(_a-_b){ return _a-_b; }
template <typename A, typename B>
inline auto multiply( const A& _a, const B& _b)->decltype(_a*_b){ return _a*_b; }

template <typename A, typename B, generic_size_type xD >
inline auto operator+( const point<A,xD>& a, const point<B,xD>& b) -> point<decltype(a.coords[0]+b.coords[0]),xD> {
	return point<decltype(a.coords[0]+b.coords[0]),xD>( typename point<decltype(a.coords[0]+b.coords[0]), xD>:: template initialize_point_data<A,B,plus<A,B> >(a.dim_values, b.dim_values) );
}

template <typename A, typename B, generic_size_type xD >
inline auto operator-( const point<A,xD>& a, const point<B,xD>& b) -> point<decltype(a.coords[0]-b.coords[0]),xD> {
	return point<decltype(a.coords[0]-b.coords[0]),xD>( typename point<decltype(a.coords[0]-b.coords[0]), xD>:: template initialize_point_data<A,B,minus<A,B> >(a.dim_values, b.dim_values) );
}

template <typename A, typename B, generic_size_type xD >
inline auto operator*( const point<A,xD>& a, const B& b) -> point<decltype(a.coords[0]*b),xD> {
	return point<decltype(a.coords[0]*b),xD>( typename point<decltype(a.coords[0]*b),xD>:: template initialize_point_scalar_data<A,B,multiply<A,B> >(a.dim_values, b) );
}
template <typename A, typename B, generic_size_type xD >
inline auto operator*( const B& b , const point<A,xD>& a ) -> point<decltype(a.coords[0]*b),xD> {
	return point<decltype(a.coords[0]*b),xD>( typename point<decltype(a.coords[0]*b),xD>:: template initialize_point_scalar_data<A,B,multiply<A,B> >(a.dim_values, b) );
}


template <typename A, typename B, generic_size_type xD >
inline point<A,xD>& operator+=( point<A,xD>& a, const point<B,xD>& b){ a.dim_values += b.dim_values; return a; }
template <typename A, generic_size_type xD >
inline point<A,xD>& operator+=( point<A,xD>& a, const A& val){ a.dim_values += val; return a; }

template <typename A, typename B, generic_size_type xD >
inline point<A,xD>& operator-=( point<A,xD>& a, const point<B,xD>& b){ a.dim_values -= b.dim_values; return a; }
template <typename A, generic_size_type xD >
inline point<A,xD>& operator-=( point<A,xD>& a, const A& val){ a.dim_values -= val; return a; }


template <typename T, generic_size_type xD >
inline std::ostream& operator<<( std::ostream& os , const point<T,xD>& p) { return ( os << "[ " << p.dim_values << " ]");}

template <typename RetT, typename ArgT, generic_size_type xD >
auto round( point<ArgT,xD> arg )->point<RetT,xD> {
	return point<RetT,xD>( typename point<RetT,xD>:: template initialize_point_data_unary<ArgT, ArgT,do_round>(arg.dim_values) );
}

}
#endif /* MPTL_POINT_H_ */
