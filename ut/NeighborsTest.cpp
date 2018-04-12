#include "matrix/HexMatrix.hpp"
#include "matrix/SquareMatrix.hpp"

#include <boost/mpl/list.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/test/unit_test.hpp>

using namespace util::matrix;

BOOST_AUTO_TEST_SUITE(NeighborsTest)

BOOST_AUTO_TEST_SUITE(Square)

using namespace square;

BOOST_AUTO_TEST_CASE(Directions) {
    Point p{3, 4};
    BOOST_CHECK_EQUAL(p + neighbors[Direction::left], (Point{2, 4}));
    BOOST_CHECK_EQUAL(p + neighbors[Direction::up], (Point{3, 3}));
    BOOST_CHECK_EQUAL(p + neighbors[Direction::right], (Point{4, 4}));
    BOOST_CHECK_EQUAL(p + neighbors[Direction::down], (Point{3, 5}));
}

BOOST_AUTO_TEST_CASE(OppositeDirection) {
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::left),
            Direction::right);
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::up),
            Direction::down);
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::right),
            Direction::left);
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::down),
            Direction::up);
}

BOOST_AUTO_TEST_CASE(Iterate) {
    Neighbors::Range range;
    auto iterator = range.begin();
    BOOST_TEST(*iterator == Direction::left);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_TEST(*iterator == Direction::up);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_TEST(*iterator == Direction::right);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_TEST(*iterator == Direction::down);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_CHECK(iterator == range.end());
}

BOOST_AUTO_TEST_CASE(GetDirection) {
    BOOST_CHECK_EQUAL(neighbors.getDirection(p10), Direction::right);
    BOOST_CHECK_EQUAL(neighbors.getDirection(p01), Direction::down);
    BOOST_CHECK_EQUAL(neighbors.getDirection(-p10), Direction::left);
    BOOST_CHECK_EQUAL(neighbors.getDirection(-p01), Direction::up);
    BOOST_CHECK_EQUAL(neighbors.getDirection(p11), boost::none);
    BOOST_CHECK_EQUAL(neighbors.getDirection(-p11), boost::none);
    BOOST_CHECK_EQUAL(neighbors.getDirection(px), boost::none);
    BOOST_CHECK_EQUAL(neighbors.getDirection(-px), boost::none);
}

BOOST_AUTO_TEST_CASE(PreviousDirection) {
    BOOST_CHECK_EQUAL(neighbors.previousDirection(Direction::left),
            Direction::down);
    BOOST_CHECK_EQUAL(neighbors.previousDirection(Direction::up),
            Direction::left);
    BOOST_CHECK_EQUAL(neighbors.previousDirection(Direction::right),
            Direction::up);
    BOOST_CHECK_EQUAL(neighbors.previousDirection(Direction::down),
            Direction::right);
}

BOOST_AUTO_TEST_CASE(NextDirection) {
    BOOST_CHECK_EQUAL(neighbors.nextDirection(Direction::left),
            Direction::up);
    BOOST_CHECK_EQUAL(neighbors.nextDirection(Direction::up),
            Direction::right);
    BOOST_CHECK_EQUAL(neighbors.nextDirection(Direction::right),
            Direction::down);
    BOOST_CHECK_EQUAL(neighbors.nextDirection(Direction::down),
            Direction::left);
}

BOOST_AUTO_TEST_SUITE_END() // Square

BOOST_AUTO_TEST_SUITE(Hex)

using namespace hex;

BOOST_AUTO_TEST_CASE(OddColumn) {
    Point p{3, 4};
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::leftDown], (Point{2, 5}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::leftUp], (Point{2, 4}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::up], (Point{3, 3}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::rightUp], (Point{4, 4}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::rightDown], (Point{4, 5}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::down], (Point{3, 5}));
}

BOOST_AUTO_TEST_CASE(EvenColumn) {
    Point p{8, 4};
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::leftDown], (Point{7, 4}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::leftUp], (Point{7, 3}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::up], (Point{8, 3}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::rightUp], (Point{9, 3}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::rightDown], (Point{9, 4}));
    BOOST_CHECK_EQUAL(p + getNeighbors(p)[Direction::down], (Point{8, 5}));
}

BOOST_AUTO_TEST_CASE(OppositeDirection) {
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::leftDown),
            Direction::rightUp);
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::leftUp),
            Direction::rightDown);
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::up),
            Direction::down);
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::rightUp),
            Direction::leftDown);
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::rightDown),
            Direction::leftUp);
    BOOST_CHECK_EQUAL(Neighbors::oppositeDirection(Direction::down),
            Direction::up);
}

BOOST_AUTO_TEST_CASE(Iterate) {
    Neighbors::Range range;
    auto iterator = range.begin();
    BOOST_TEST(*iterator == Direction::leftDown);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_TEST(*iterator == Direction::leftUp);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_TEST(*iterator == Direction::up);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_TEST(*iterator == Direction::rightUp);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_TEST(*iterator == Direction::rightDown);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_TEST(*iterator == Direction::down);
    BOOST_CHECK(iterator != range.end());
    ++iterator;
    BOOST_CHECK(iterator == range.end());
}

// TODO: previousDirection and nextDirection

BOOST_AUTO_TEST_SUITE_END() // Hex

using NeighborTypes = boost::mpl::list<square::Neighbors, hex::Neighbors>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IteratorOperations, Neighbors, NeighborTypes) {
    typename Neighbors::Range range;
    auto it1 = range.begin();
    auto it2 = range.begin();
    auto it3 = it1;
    BOOST_CHECK(it1 == it2);
    BOOST_CHECK(it1 == it3);
    BOOST_CHECK(it2 == it3);

    ++it1;
    ++it2;
    BOOST_CHECK(it1 == it2);
    BOOST_CHECK(it1 != it3);
    BOOST_CHECK(it2 != it3);
    BOOST_CHECK_EQUAL(it3 - it1, -1);

    ++it1;
    BOOST_CHECK_EQUAL(it1 - it3, 2);

    it1 -= 2;
    BOOST_CHECK(it1 == it3);
}

BOOST_AUTO_TEST_SUITE_END() // NeighborsTest
