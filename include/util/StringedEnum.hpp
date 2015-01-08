#ifndef INCLUDE_UTIL_STRINGEDENUM_HPP
#define INCLUDE_UTIL_STRINGEDENUM_HPP

#include <ostream>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/stringize.hpp>

#define _STRINGED_ENUM_ENUM_OUTPUT_HELPER(r, Type, elem) \
	case Type::elem: return os << BOOST_PP_STRINGIZE(elem);
#define _STRINGED_ENUM_HELPER(r, Type, elem) \
	elem,

#define STRINGED_ENUM(Type, values) \
	enum class Type { \
			BOOST_PP_SEQ_FOR_EACH( \
				_STRINGED_ENUM_HELPER, \
				Type, values) \
	}; \
	inline ::std::ostream& operator<<(::std::ostream& os, Type value) { \
		switch (value) { \
			BOOST_PP_SEQ_FOR_EACH( \
				_STRINGED_ENUM_ENUM_OUTPUT_HELPER, \
				Type, values) \
		}; \
		return os; \
	}

#endif /* INCLUDE_UTIL_STRINGEDENUM_HPP */
