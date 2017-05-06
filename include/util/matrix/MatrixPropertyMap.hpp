#ifndef TOOLS_MATRIX_MATRIXPROPERTYMAP_HPP
#define TOOLS_MATRIX_MATRIXPROPERTYMAP_HPP

#include "Matrix.hpp"

#include <boost/property_map/property_map.hpp>

namespace util {
namespace matrix {

template<typename T>
struct MatrixPropertyMap {
    Matrix<T>& matrix;
};

template<typename T>
auto matrixPropertyMap(Matrix<T>& matrix) {
    return MatrixPropertyMap<T>{matrix};
}

namespace boost {

template<typename T>
struct property_traits<MatrixPropertyMap<T>> {
    using category = lvalue_property_map_tag;
    using key_type = Point;
    using value_type = T;
    using reference = typename Matrix<T>::reference;
};

} // namespace boost

template<typename T>
typename Matrix<T>::reference get(MatrixPropertyMap<T>& map, Point p) {
    return map.matrix[p];
}

template<typename T>
typename Matrix<T>::const_reference get(const MatrixPropertyMap<T>& map,
        Point p) {
    return map.matrix[p];
}

template<typename T>
void put(MatrixPropertyMap<T>& map, Point p, T t) {
    map.matrix[p] = std::move(t);
}

} // namespace matrix
} // namespace util

#endif // TOOLS_MATRIX_MATRIXPROPERTYMAP_HPP
