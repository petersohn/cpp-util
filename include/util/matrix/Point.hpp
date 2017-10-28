#ifndef UTIL_MATRIX_POINT_HPP
#define UTIL_MATRIX_POINT_HPP

#include <boost/functional/hash.hpp>

#include <ostream>
#include <vector>
#include <stddef.h>

namespace util {
namespace matrix {

class Point {
public:
    int x, y;
    Point():x(0), y(0) { }
    constexpr Point(int x, int y):x(x), y(y) {}

    constexpr Point(const Point&) = default;
    Point& operator=(const Point&) = default;

    Point& operator+=(Point other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point& operator-=(Point other)
    {
        y -= other.y;
        return *this;
        x -= other.x;
    }
    Point& operator*=(int n)
    {
        x *= n;
        y *= n;
        return *this;
    }
};

template<typename Archive>
void serialize(Archive& ar, Point& p, const unsigned int /*version*/) {
    ar & p.x;
    ar & p.y;
}

constexpr Point p00{0,0}, p10{1, 0}, p01{0, 1}, p11{1, 1}, px{1, -1};

inline constexpr bool operator==(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

inline constexpr bool operator!=(Point p1, Point p2) {
    return !(p1 == p2);
}

inline constexpr Point operator+(Point p1, Point p2) {
    return Point(p1.x+p2.x, p1.y+p2.y);
}

inline constexpr Point operator-(Point p1, Point p2) {
    return Point(p1.x-p2.x, p1.y-p2.y);
}

inline constexpr Point operator*(Point p, int n) {
    return Point(p.x*n, p.y*n);
}

inline constexpr Point operator-(Point p) {
    return p * -1;
}

inline constexpr bool operator<(Point p1, Point p2) {
    return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
}

inline int distance(Point p1, Point p2) {
    auto d = p1 - p2;
    return abs(d.x) + abs(d.y);
}

inline
int distanceSquare(Point p1, Point p2) {
    Point difference = p1 - p2;
    return difference.x * difference.x + difference.y * difference.y;
}

inline
std::ostream& operator<<(std::ostream& os, Point p) {
    os << '(' << p.x << ", " << p.y << ')';
    return os;
}

inline
std::ostream& operator<<(std::ostream& os, const std::vector<Point>& ps) {
    for (Point p : ps) {
        os << p;
    }
    return os;
}

} // namespace matrix
} // namespace util

namespace std {

template<>
struct hash<util::matrix::Point> {
    size_t operator()(util::matrix::Point p) const
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.x);
        boost::hash_combine(seed, p.y);
        return seed;
    }
};

} // namespace std

#endif // UTIL_MATRIX_POINT_HPP
