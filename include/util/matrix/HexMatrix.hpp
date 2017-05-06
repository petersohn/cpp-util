#ifndef TOOLS_MATRIX_HEXMATRIX_HPP
#define TOOLS_MATRIX_HEXMATRIX_HPP

#include "Matrix.hpp"

#include <istream>
#include <ostream>

namespace util {
namespace matrix {
namespace hex {

template<typename T>
void printMatrix(std::ostream& os, const Matrix<T>& matrix) {
    os << "   ";
    for (int x = 0; x < static_cast<int>(matrix.width()); ++x) {
        os << std::setw(3) << x % 100;
    }
    os << "\n";
    for (int y = 0; y < static_cast<int>(2*matrix.height()); ++y) {
        if (y % 2 == 0) {
            os << std::setw(2) << (y / 2) % 100 << " ";
        } else {
            os << "   ";
        }
        for (int x = 0; x < static_cast<int>(matrix.width()); ++x) {
            if (x % 2 == y % 2) {
                os << std::setw(3) << matrix[Point{x, y / 2}];
            } else {
                os << "   ";
            }
        }
        os << "\n";
    }
}

constexpr std::size_t numNeighbors = 6;
using Neighbors = std::array<Point, numNeighbors>;

constexpr Neighbors oddNeighbors{
        {-px, -p10, -p01, p10, p11, p01}};

constexpr Neighbors evenNeighbors{
        {-p10, -p11, -p01, px, p10, p01}};

constexpr const Neighbors& getNeighbors(Point p) {
    return p.x % 2 == 0 ? evenNeighbors : oddNeighbors;
}

} // namespace hex
} // namespace matrix
} // namespace util


#endif // TOOLS_MATRIX_HEXMATRIX_HPP
