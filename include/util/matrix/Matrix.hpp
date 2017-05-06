#ifndef UTIL_MATRIX_HPP
#define UTIL_MATRIX_HPP

#include "Point.hpp"
#include "PointRange.hpp"

#include <boost/range/algorithm.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/functional/hash.hpp>

#include <assert.h>
#include <type_traits>
#include <vector>

namespace util {
namespace matrix {

template <typename Matrix>
inline bool isInsideMatrix(const Matrix& matrix, Point p) {
    return p.x >= 0 && p.y >= 0 && p.x < static_cast<int>(matrix.width()) &&
            p.y < static_cast<int>(matrix.height());
}

template<typename T>
class Matrix {
    typedef std::vector<T> Data;
    std::size_t width_, height_;
    Data data_;
public:
    typedef T valueType;
    typedef typename Data::reference reference;
    typedef typename Data::const_reference const_reference;
    typedef typename Data::iterator iterator;
    typedef typename Data::const_iterator const_iterator;

    Matrix(): width_(0), height_(0) {}
    Matrix(std::size_t width, std::size_t height, const T& defValue = T()):
        width_(width), height_(height), data_(width * height, defValue)
    {}
    Matrix(const Matrix& ) = default;
    Matrix(Matrix&& other) noexcept : width_(other.width_), height_(other.height_),
            data_(std::move(other.data_)) {
        other.width_ = 0;
        other.height_ = 0;
        other.data_.clear();
    }
    Matrix& operator=(const Matrix& ) = default;
    Matrix& operator=(Matrix&& other) noexcept {
        this->width_ = other.width_;
        this->height_ = other.height_;
        this->data_ = std::move(other.data_);
        other.width_ = 0;
        other.height_ = 0;
        other.data_.clear();
        return *this;
    }

    reference operator[](std::size_t pos) {
        return data_[pos];
    }
    const_reference operator[](std::size_t pos) const {
        assert(pos >= 0 && pos < size());
        return data_[pos];
    }
    reference operator[](Point p) {
        assert(isInsideMatrix(*this, p));
        return data_[p.y*width_ + p.x];
    }
    const_reference operator[](Point p) const {
        assert(isInsideMatrix(*this, p));
        return data_[p.y*width_ + p.x];
    }
    std::size_t size() const { return data_.size(); }
    std::size_t width() const { return width_; }
    std::size_t height() const { return height_; }
    void reset(std::size_t newWidth, std::size_t newHeight,
            const T& defValue = T())
    {
        width_ = newWidth;
        height_ = newHeight;
        data_.resize(width_ * height_);
        fill(defValue);
    }
    void fill(const T& value)
    {
        boost::fill(data_, value);
    }
    void clear()
    {
        data_.clear();
        width_ = 0;
        height_ = 0;
    }

    bool operator==(const Matrix<T>& other) const
    {
        return width_ == other.width_ && height_ == other.height_
                && data_ == other.data_;
    }

    iterator begin() { return data_.begin(); }
    iterator end() { return data_.end(); }
    const_iterator begin() const { return data_.begin(); }
    const_iterator end() const { return data_.end(); }
    const_iterator cbegin() const { return data_.cbegin(); }
    const_iterator cend() const { return data_.cend(); }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
        ar & width_;
        ar & height_;
        ar & data_;
    }
};

template<typename T>
inline const T matrixAt(const Matrix<T> &arr,
        Point p, const T& def) {
    return isInsideMatrix(arr, p) ? arr[p] : def;
}

template <typename Matrix>
inline PointRange matrixRange(const Matrix& matrix) {
    return PointRange(Point(0,0), Point(matrix.width(), matrix.height()));
}

template<typename T>
inline bool operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return !(lhs == rhs);
}

} // namespace matrix
} // namespace util

namespace std {
template<typename T>
struct hash<util::matrix::Matrix<T>> {
    size_t operator()(const util::matrix::Matrix<T>& arr) const {
        size_t seed = 0;
        for (const T& t : arr) {
            boost::hash_combine(seed, t);
        }
        return seed;
    }
};

}

#endif // UTIL_MATRIX_HPP
