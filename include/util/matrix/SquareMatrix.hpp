#ifndef UTIL_MATRIX_SQUAREMATRIX_HPP
#define UTIL_MATRIX_SQUAREMATRIX_HPP

#include "Point.hpp"

namespace util {
namespace matrix {
namespace square {

LAZY_ARGUMENT_ENUM(Direction, directions, (left)(up)(right)(down));

constexpr std::size_t numNeighbors = 4;
using Neighbors = NeighborsBase<Direction, numNeighbors>;

constexpr Neighbors neighbors{
        {{-p10, -p01, p10, p01}}};

} // square
} // namespace matrix
} // namespace util

#endif // UTIL_MATRIX_SQUAREMATRIX_HPP
