#ifndef UTIL_MATRIX_POINTRANGE_HPP
#define UTIL_MATRIX_POINTRANGE_HPP

#include "Point.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/exception/all.hpp>

#include <stdexcept>

namespace util {
namespace matrix {

class PointRangeIterator;

class PointRange {
public:
    typedef PointRangeIterator iterator;
private:
    Point begin_;
    Point end_;
public:
    typedef Point value_type;
    friend class PointRangeIterator;

    PointRange(Point  begin, Point  end):
        begin_(begin),
        end_(end) {
    }

    iterator begin() const;
    iterator end() const;
    Point  front() const
    {
        if (begin_ == end_) {
            BOOST_THROW_EXCEPTION(std::out_of_range(
                    "front() cannot be called on empty PointRange."));
        }
        return begin_;
    }
    Point back() const
    {
        if (begin_ == end_) {
            BOOST_THROW_EXCEPTION(std::out_of_range(
                    "back() cannot be called on empty PointRange."));
        }
        return Point(end_.x - 1, end_.y - 1);
    }
}; // class PointRange

class PointRangeIterator: public boost::iterator_facade<
        PointRangeIterator,
        Point,
        boost::bidirectional_traversal_tag,
        Point > {
public:
    PointRangeIterator(const PointRangeIterator&) = default;
    PointRangeIterator(PointRangeIterator&&) = default;

    PointRangeIterator& operator=(const PointRangeIterator&) = default;
    PointRangeIterator& operator=(PointRangeIterator&&) = default;

private:
    const PointRange* owner_;
    Point p_;

    friend class PointRange;
    friend class boost::iterator_core_access;

    PointRangeIterator(const PointRange& owner, Point  p):
        owner_(&owner),
        p_(p)
    {}
    Point  dereference() const { return p_; }
    void increment()
    {
        ++p_.x;
        if (p_.x == owner_->end_.x) {
            p_.x = owner_->begin_.x;
            ++p_.y;
        }
    }
    void decrement()
    {
        if (p_.x == owner_->begin_.x) {
            p_.x = owner_->end_.x - 1;
            --p_.y;
        } else {
            --p_.x;
        }
    }

    bool equal(const PointRangeIterator& other) const
    {
        return p_ == other.p_;
    }
}; // class PointRangeIterator

inline
PointRange::iterator PointRange::begin() const
{
    return iterator(*this, begin_);
}

inline
PointRange::iterator PointRange::end() const
{
    return iterator(*this, Point(begin_.x, end_.y));
}

} // namespace matrix
} // namespace util

#endif // UTIL_MATRIX_POINTRANGE_HPP
