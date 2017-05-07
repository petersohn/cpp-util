#ifndef UTIL_DEFAULTVALUE_HPP
#define UTIL_DEFAULTVALUE_HPP

#include <boost/program_options/value_semantic.hpp>

namespace util {

template <typename T>
boost::program_options::typed_value<T>* defaultValue(T& v) {
    return boost::program_options::value(&v)->default_value(v);
}

} // namespace util

#endif // UTIL_DEFAULTVALUE_HPP
