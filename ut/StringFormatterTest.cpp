#include "util/StringFormatter.hpp"
#include <boost/test/unit_test.hpp>

using namespace util;

BOOST_AUTO_TEST_SUITE(StringFormatterTest)

BOOST_AUTO_TEST_CASE(parse_simple_action)
{
    std::string actionString = "some value";
    StringFormatter::Map actions{
            {"first", [&](const std::vector<std::string>&) { return actionString; }}
        };

    StringFormatter stringFormatterUnderTest{actions};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("%first%"),
            actionString);
}

BOOST_AUTO_TEST_SUITE_END()

