#ifndef SRC_LAZYARGUMENTENUM_HPP
#define SRC_LAZYARGUMENTENUM_HPP

#include "util/StringEnumValue.hpp"
#include "util/LazyArgumentMap.hpp"
#include "util/PrefixMap.hpp"
#include "util/StringedEnum.hpp"
#include <boost/preprocessor/seq/for_each.hpp>
#include <istream>
#include <ostream>

#define _LAZY_ARGUMENT_ENUM_PREFIX_MAP_HELPER(r, Type, elem) \
	STRING_ENUM_VALUE(Type, elem),

#define LAZY_ARGUMENT_ENUM(Type, name, values) \
	STRINGED_ENUM(Type, values) \
	LAZY_ARGUMENT_PREFIX_MAP(Type, name) { \
		return { BOOST_PP_SEQ_FOR_EACH( \
				_LAZY_ARGUMENT_ENUM_PREFIX_MAP_HELPER, \
				Type, values) \
		}; \
	} \
	inline ::std::istream& operator>>(::std::istream& is, Type& value) { \
		::std::string s; \
		is >> s; \
		value = name().at(s); \
		return is; \
	}



#endif /* SRC_LAZYARGUMENTENUM_HPP */
