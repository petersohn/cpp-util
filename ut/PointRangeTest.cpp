#include "matrix/PointRange.hpp"

#include <boost/test/unit_test.hpp>

#include <unordered_set>

using namespace util::matrix;

BOOST_AUTO_TEST_SUITE(PointRangeTest)

BOOST_AUTO_TEST_CASE(Endpoints)
{
    Point begin(1,0);
    Point end(3,4);
    PointRange range(begin, end);
    BOOST_CHECK_EQUAL(range.front(), begin);
    BOOST_CHECK_EQUAL(range.back(), end - p11);
}

BOOST_AUTO_TEST_CASE(EmptyRange)
{
    Point begin(2,1);
    Point end(2,1);
    PointRange range(begin, end);
    BOOST_CHECK(range.begin() == range.end());
    BOOST_CHECK_THROW(range.front(), std::out_of_range);
    BOOST_CHECK_THROW(range.back(), std::out_of_range);
}


BOOST_AUTO_TEST_CASE(SinglePointRange)
{
    Point p(1,10);
    PointRange range(p, p+p11);
    BOOST_CHECK_EQUAL(range.front(), p);
    BOOST_CHECK_EQUAL(range.back(), p);
    PointRange::iterator it = range.begin();
    BOOST_CHECK_EQUAL(*it, p);
    BOOST_CHECK(++it == range.end());
}

BOOST_AUTO_TEST_CASE(InRange)
{
    Point begin(2,2);
    Point end(5,7);
    PointRange range(begin, end);
    std::unordered_set<Point> pointsConsidered;
    for (Point  p: range) {
        BOOST_TEST_MESSAGE(p);
        BOOST_REQUIRE_GE(p.x, begin.x);
        BOOST_REQUIRE_LT(p.x, end.x);
        BOOST_REQUIRE_GE(p.y, begin.y);
        BOOST_REQUIRE_LT(p.y, end.y);
        BOOST_REQUIRE_EQUAL(pointsConsidered.count(p), 0);
        pointsConsidered.insert(p);
    }
}

BOOST_AUTO_TEST_SUITE(Iterators)

BOOST_AUTO_TEST_CASE(PrePlus)
{
    Point begin(2,2);
    Point end(4,5);
    PointRange range(begin, end);
    PointRange::iterator it = range.begin();
    BOOST_CHECK_EQUAL(*it, Point(2,2));
    BOOST_CHECK_EQUAL(*++it, Point(3,2));
    BOOST_CHECK_EQUAL(*++it, Point(2,3));
    BOOST_CHECK_EQUAL(*++it, Point(3,3));
    BOOST_CHECK_EQUAL(*++it, Point(2,4));
    BOOST_CHECK_EQUAL(*++it, Point(3,4));
    BOOST_CHECK(++it == range.end());
}

BOOST_AUTO_TEST_CASE(PreMinus)
{
    Point begin(2,2);
    Point end(4,5);
    PointRange range(begin, end);
    PointRange::iterator it = range.end();
    BOOST_CHECK(it == range.end());
    BOOST_CHECK_EQUAL(*--it, Point(3,4));
    BOOST_CHECK_EQUAL(*--it, Point(2,4));
    BOOST_CHECK_EQUAL(*--it, Point(3,3));
    BOOST_CHECK_EQUAL(*--it, Point(2,3));
    BOOST_CHECK_EQUAL(*--it, Point(3,2));
    BOOST_CHECK_EQUAL(*--it, Point(2,2));
}

BOOST_AUTO_TEST_CASE(PostPlus)
{
    Point begin(2,2);
    Point end(4,5);
    PointRange range(begin, end);
    PointRange::iterator it = range.begin();
    BOOST_CHECK_EQUAL(*it++, Point(2,2));
    BOOST_CHECK_EQUAL(*it++, Point(3,2));
    BOOST_CHECK_EQUAL(*it++, Point(2,3));
    BOOST_CHECK_EQUAL(*it++, Point(3,3));
    BOOST_CHECK_EQUAL(*it++, Point(2,4));
    BOOST_CHECK_EQUAL(*it++, Point(3,4));
    BOOST_CHECK(it == range.end());
}

BOOST_AUTO_TEST_CASE(PostMinus)
{
    Point begin(2,2);
    Point end(4,5);
    PointRange range(begin, end);
    PointRange::iterator it = range.end();
    BOOST_CHECK(it-- == range.end());
    BOOST_CHECK_EQUAL(*it--, Point(3,4));
    BOOST_CHECK_EQUAL(*it--, Point(2,4));
    BOOST_CHECK_EQUAL(*it--, Point(3,3));
    BOOST_CHECK_EQUAL(*it--, Point(2,3));
    BOOST_CHECK_EQUAL(*it--, Point(3,2));
    BOOST_CHECK_EQUAL(*it, Point(2,2));
}

BOOST_AUTO_TEST_SUITE_END() // Iterators


BOOST_AUTO_TEST_SUITE_END() // PointRangeTest
