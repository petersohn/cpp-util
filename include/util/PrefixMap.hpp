#ifndef SRC_PREFIXMAP_HPP
#define SRC_PREFIXMAP_HPP

#include "PrefixMapTree.hpp"

namespace util {

template <typename Char, typename Value>
using BasicPrefixMap = BasicPrefixMapTree<Char, Value>;

template <typename Value>
using PrefixMap = BasicPrefixMap<char, Value>;

} // namespace util

#endif /* SRC_PREFIXMAP_HPP */
