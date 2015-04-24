#include "ContainerWrapper.hpp"
#include <vector>
#include <set>
#include <boost/test/unit_test.hpp>

using namespace util;

BOOST_AUTO_TEST_SUITE( ContainerWrapperTest )

BOOST_AUTO_TEST_CASE(compare_equal_vectors)
{
    std::vector<int> v1{1, 2, 3, 2, 0};
    std::vector<int> v2 = v1;

    BOOST_CHECK_EQUAL(wrap(v1), wrap(v2));
}

BOOST_AUTO_TEST_CASE(compare_non_equal_vectors)
{
    std::vector<int> v1{1, 2, 3, 2, 0};
    std::vector<int> v2{0, 1, 0};

    BOOST_CHECK_NE(wrap(v1), wrap(v2));
}

BOOST_AUTO_TEST_CASE(compare_equal_sets)
{
    std::set<int> s1{1, 4, 0};
    std::set<int> s2 = s1;

    BOOST_CHECK_EQUAL(wrap(s1), wrap(s2));
}

BOOST_AUTO_TEST_CASE(compare_non_equal_sets)
{
    std::set<int> s1{1, 4, 0};
    std::set<int> s2;

    BOOST_CHECK_NE(wrap(s1), wrap(s2));
}

BOOST_AUTO_TEST_SUITE_END() // demangleTest




