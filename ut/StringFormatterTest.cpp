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

BOOST_AUTO_TEST_CASE(parse_simple_action_with_constant_parts)
{
    std::string actionString = "some value";
    std::string prefix = "prefix string";
    std::string suffix = "suffix string";
    StringFormatter::Map actions{
            {"first", [&](const std::vector<std::string>&) { return actionString; }}
        };

    StringFormatter stringFormatterUnderTest{actions};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString(
                prefix + "%first%" + suffix),
            prefix + actionString + suffix);
}

BOOST_AUTO_TEST_CASE(parse_more_actions)
{
    std::string actionString1 = "some value1";
    std::string actionString2 = "some value2";
    std::string middleString = "__";
    StringFormatter::Map actions{
            {"first", [&](const std::vector<std::string>&) { return actionString1; }},
            {"second", [&](const std::vector<std::string>&) { return actionString2; }}
        };

    StringFormatter stringFormatterUnderTest{actions};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("%first%__%second%"),
            actionString1 + middleString + actionString2);
}

BOOST_AUTO_TEST_CASE(parse_action_with_parameters)
{
    std::string actionString = "some value";
    std::string argument1 = "arg1";
    std::string argument2 = "arg2";
    StringFormatter::Map actions{
            {"first", [&](const std::vector<std::string>& args)
                {
                    std::string result = actionString;
                    for (const auto& arg: args) {
                        result += " " + arg;
                    }
                    return result;
                }}
        };

    StringFormatter stringFormatterUnderTest{actions};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString(
                "%first:" + argument1 + ":" + argument2 + "%"),
            actionString + " " + argument1 + " " + argument2);
}



BOOST_AUTO_TEST_SUITE_END()

