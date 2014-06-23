/*
 * flag_holder.h
 *
 *  Created on: 22 mar 2013
 *      Author: Johan
 */

#ifndef FLAG_HOLDER_H_
#define FLAG_HOLDER_H_

#include <ostream>

namespace mptl{


template< typename enum_type >
class flags_holder{
	unsigned int flag_storage;
	static constexpr unsigned int flag_value( const enum_type& flag ){ return static_cast<unsigned int>(flag); }
	static constexpr unsigned int flag_mask_impl( const enum_type& flag ){ return 1 << flag_value(flag); }
	static constexpr unsigned int flag_mask_impl( const flags_holder& flag ){ return flag.flag_storage; }

	static constexpr unsigned int agregate_flag_mask( const flags_holder& flag ){ return flag_mask_impl(flag); }
	static constexpr unsigned int agregate_flag_mask( const enum_type&  flag ){ return flag_mask_impl(flag); }

	template< typename... Args >
	static constexpr unsigned int agregate_flag_mask( const flags_holder& first, Args... rest )
	{ return flag_mask_impl(first) | agregate_flag_mask(rest...); }

	template< typename... Args >
	static constexpr unsigned int agregate_flag_mask( const enum_type& first, Args... rest )
	{ return flag_mask_impl(first) | agregate_flag_mask(rest...); }

	template< typename T >
	void   set_flag_impl( T flag ){ flag_storage |=  flag_mask_impl( flag ); }
	template< typename T >
	void unset_flag_impl( T flag ){ flag_storage &= ~flag_mask_impl( flag ); }

	template < typename T >
	friend std::ostream& operator << (std::ostream& os, const flags_holder<T>& holder );

public:
	constexpr flags_holder( ):flag_storage( 0 ){ }
	constexpr flags_holder( const flags_holder& holder):flag_storage( holder.flag_storage ){ }
	template< typename... Args >
	explicit constexpr flags_holder( Args... flag_tuple ) : flag_storage( agregate_flag_mask(flag_tuple...) ){ }

	template<typename Arg, typename... Args>
	void set_flags( Arg flag, Args... flag_tuple ){ set_flag_impl(flag); set_flags(flag_tuple...); }

	void   set_flag( enum_type f ){   set_flag_impl(f); }
	void unset_flag( enum_type f ){ unset_flag_impl(f); }

	bool is_set( enum_type f ) const { return ( flag_mask_impl(f) & flag_storage ) != 0; };

	flags_holder& operator |= ( const enum_type& flag ) { set_flag(flag); return *this; }
	constexpr unsigned int value() const { return flag_storage; }
};

template<typename enum_type, typename... Args>
static constexpr unsigned int value(Args... flag_tuple){
	return flags_holder<enum_type>(flag_tuple...).value();
}

template< typename enum_type >
std::ostream& operator << (std::ostream& os, const flags_holder<enum_type>& holder ){
	return ( os << holder.flag_storage );
}

}

#endif /* FLAG_HOLDER_H_ */
