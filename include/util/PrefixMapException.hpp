#ifndef INCLUDE_UTIL_PREFIXMAPEXCEPTION_HPP
#define INCLUDE_UTIL_PREFIXMAPEXCEPTION_HPP

#include <stdexcept>

namespace util {

struct PrefixMapError: std::logic_error {
    using std::logic_error::logic_error;
};

struct AmbiguousValue: PrefixMapError {
    AmbiguousValue(): PrefixMapError{"Ambiguous value"} {}
};

struct ValueNotFound: PrefixMapError {
    ValueNotFound(): PrefixMapError{"Value not found"} {}
};

struct DuplicateValue: PrefixMapError {
    DuplicateValue(): PrefixMapError{"Duplicate value or prefix of existing value"} {}
};

} // namespace util


#endif /* INCLUDE_UTIL_PREFIXMAPEXCEPTION_HPP */
