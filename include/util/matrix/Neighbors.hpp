#ifndef UTIL_MATRIX_DIRECTION_HPP
#define UTIL_MATRIX_DIRECTION_HPP

#include "Point.hpp"

#include <boost/iterator/iterator_facade.hpp>

#include <array>

namespace util {
namespace matrix {

template<typename Direction_, std::size_t numNeighbors_>
class NeighborIterator : public boost::iterator_facade<
        NeighborIterator<Direction_, numNeighbors_>,
        Direction_, boost::random_access_traversal_tag, Direction_> {
public:
    using Direction = Direction_;
    static constexpr int numNeighbors = numNeighbors_;

    template<typename D, std::size_t N>
    friend struct NeighborRange;

    friend class boost::iterator_core_access;
private:
    Direction dereference() const {
        return static_cast<Direction>(value);
    }

    bool equal(const NeighborIterator& other) const {
        return other.value == value;
    }

    void increment() { ++ value; }
    void decrement() { -- value; }
    void advance(int n) { value += n; }
    int distance_to(const NeighborIterator& other) const {
        return other.value - value;
    }

    NeighborIterator(int value) : value(value) {}

    int value;
};

template<typename Direction_, std::size_t numNeighbors_>
struct NeighborRange {
    using Direction = Direction_;
    static constexpr int numNeighbors = numNeighbors_;

    using iterator = NeighborIterator<Direction, numNeighbors>;
    using const_iterator = iterator;

    iterator begin() const {
        return iterator{0};
    }

    iterator end() const {
        return iterator{numNeighbors};
    }
};

template<typename Direction_, std::size_t numNeighbors_>
struct NeighborsBase {
    using Direction = Direction_;
    static constexpr int numNeighbors = numNeighbors_;
    using Range = NeighborRange<Direction, numNeighbors>;

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
