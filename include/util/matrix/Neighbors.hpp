#ifndef UTIL_MATRIX_DIRECTION_HPP
#define UTIL_MATRIX_DIRECTION_HPP

#include "Point.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>

#include <algorithm>
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

    constexpr Point operator[](Direction direction) const {
        return data[static_cast<std::size_t>(direction)];
    }

    boost::optional<Direction> getDirection(Point d) const {
        auto iterator = std::find(data.begin(), data.end(), d);
        if (iterator != data.end()) {
            return static_cast<Direction>(
                    std::distance(data.begin(), iterator));
        } else {
            return boost::none;
        }
    }

    static Direction oppositeDirection(Direction direction) {
        return static_cast<Direction>(
                (static_cast<std::size_t>(direction) + numNeighbors / 2)
                % numNeighbors);
    }

    static Direction previousDirection(Direction direction) {
        return static_cast<Direction>(
                (static_cast<std::size_t>(direction) + numNeighbors - 1)
                % numNeighbors);
    }

    static Direction nextDirection(Direction direction) {
        return static_cast<Direction>(
                (static_cast<std::size_t>(direction) + 1)
                % numNeighbors);
    }
};

} // namespace matrix
} // namespace util

#endif // UTIL_MATRIX_DIRECTION_HPP
