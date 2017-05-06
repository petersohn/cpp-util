#ifndef UTIL_MATRIX_DIRECTION_HPP
#define UTIL_MATRIX_DIRECTION_HPP

#include "Point.hpp"

#include <array>

namespace util {
namespace matrix {

template<typename Direction, std::size_t numNeighbors>
struct NeighborsBase {
    std::array<Point, numNeighbors> data;
    Point operator[](Direction direction) const {
        return data[static_cast<std::size_t>(direction)];
    }

    static Direction oppositeDirection(Direction direction) {
        return static_cast<Direction>(
                (static_cast<std::size_t>(direction) + numNeighbors / 2)
                % numNeighbors);
    }
};

} // namespace matrix
} // namespace util

#endif // UTIL_MATRIX_DIRECTION_HPP
