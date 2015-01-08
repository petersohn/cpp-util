#include "util/StringFormatter.hpp"
#include <boost/test/unit_test.hpp>

using namespace util;

BOOST_AUTO_TEST_SUITE(StringFormatterTest)

BOOST_AUTO_TEST_CASE(format_simple_action)
{
    std::string actionString = "some value";
    StringFormatter::Map actions{
            {"first", [&](const std::vector<std::string>&) { return actionString; }}
        };

    StringFormatter stringFormatterUnderTest{actions};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("%first%"),
            actionString);
}

BOOST_AUTO_TEST_CASE(format_simple_action_with_constant_parts)
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

BOOST_AUTO_TEST_CASE(format_more_actions)
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

BOOST_AUTO_TEST_CASE(format_action_with_parameters)
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

BOOST_AUTO_TEST_CASE(more_actions_with_arguments)
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
                "%first:" + argument1 + "%_%first:" + argument2 + "%"),
            actionString + " " + argument1 + "_" + actionString + " " + argument2);
}

BOOST_AUTO_TEST_CASE(escape_sequence)
{
    StringFormatter::EscapeMap escapeCharacters{
            {'a', "escape sequence"}};
    StringFormatter::Map actions;

    StringFormatter stringFormatterUnderTest{actions, '%', ':', '\\',
            escapeCharacters};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("\\a"),
            escapeCharacters.at('a'));
}

BOOST_AUTO_TEST_CASE(text_before_escape_sequence)
{
    StringFormatter::EscapeMap escapeCharacters{
            {'a', "escape sequence"}};
    StringFormatter::Map actions;
    std::string textPrefix = "text prefix";

    StringFormatter stringFormatterUnderTest{actions, '%', ':', '\\',
            escapeCharacters};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString(textPrefix + "\\a"),
            textPrefix + escapeCharacters.at('a'));
}

BOOST_AUTO_TEST_CASE(text_after_escape_sequence)
{
    StringFormatter::EscapeMap escapeCharacters{
            {'a', "escape sequence"}};
    StringFormatter::Map actions;
    std::string textSuffix= "text suffix";

    StringFormatter stringFormatterUnderTest{actions, '%', ':', '\\',
            escapeCharacters};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("\\a" + textSuffix),
            escapeCharacters.at('a') + textSuffix);
}

BOOST_AUTO_TEST_CASE(multiple_escape_sequences)
{
    StringFormatter::EscapeMap escapeCharacters{
            {'a', "escape sequence"},
            {'x', "another escape sequence"}};
    StringFormatter::Map actions;
    std::string textPrefix = "text prefix";
    std::string textMiddle = "text middle";
    std::string textSuffix = "text suffix";

    StringFormatter stringFormatterUnderTest{actions, '%', ':', '\\',
            escapeCharacters};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString(
            textPrefix + "\\x" + textMiddle + "\\a" + textSuffix),
            textPrefix + escapeCharacters.at('x') + textMiddle +
            escapeCharacters.at('a') + textSuffix);
}

BOOST_AUTO_TEST_CASE(default_escape_characters)
{
    StringFormatter::Map actions;

    StringFormatter stringFormatterUnderTest{actions};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("\\a"), "\a");
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("\\b"), "\b");
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("\\f"), "\f");
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("\\n"), "\n");
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("\\r"), "\r");
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("\\t"), "\t");
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("\\\\"), "\\");
}

BOOST_AUTO_TEST_CASE(different_argument_char)
{
    std::string actionString = "some value";
    StringFormatter::Map actions{
            {"first", [&](const std::vector<std::string>&) { return actionString; }}
        };

    StringFormatter stringFormatterUnderTest{actions, '_'};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("_first_"),
            actionString);
}

BOOST_AUTO_TEST_CASE(different_argument_separator)
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

    StringFormatter stringFormatterUnderTest{actions, '%', '!'};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString(
                "%first!" + argument1 + "!" + argument2 + "%"),
            actionString + " " + argument1 + " " + argument2);
}

BOOST_AUTO_TEST_CASE(different_escape_character)
{
    StringFormatter::EscapeMap escapeCharacters{
            {'a', "escape sequence"}};
    StringFormatter::Map actions;

    StringFormatter stringFormatterUnderTest{actions, '%', ':', '/',
            escapeCharacters};
    BOOST_CHECK_EQUAL(stringFormatterUnderTest.formatString("/a"),
            escapeCharacters.at('a'));
}

BOOST_AUTO_TEST_CASE(throw_if_action_not_found)
{
    std::string actionString = "some value";
    StringFormatter::Map actions{
            {"first", [&](const std::vector<std::string>&) { return actionString; }}
        };

    StringFormatter stringFormatterUnderTest{actions};
    BOOST_CHECK_THROW(stringFormatterUnderTest.formatString("%invalid%"),
            InvalidAction);
}

BOOST_AUTO_TEST_CASE(throw_if_action_not_terminated)
{
    std::string actionString = "some value";
    StringFormatter::Map actions{
            {"first", [&](const std::vector<std::string>&) { return actionString; }}
        };

    StringFormatter stringFormatterUnderTest{actions};
    BOOST_CHECK_THROW(stringFormatterUnderTest.formatString("%first"),
            UnterminatedAction);
}

BOOST_AUTO_TEST_CASE(throw_if_escape_sequence_is_invalid)
{
    StringFormatter::EscapeMap escapeCharacters{
            {'a', "escape sequence"}};
    StringFormatter::Map actions;
    StringFormatter stringFormatterUnderTest{actions, '%', ':', '\\',
            escapeCharacters};

    BOOST_CHECK_THROW(stringFormatterUnderTest.formatString("\\x"),
            InvalidEscapeSequence);
}

BOOST_AUTO_TEST_CASE(throw_if_escape_sequence_is_not_terminated)
{
    StringFormatter::EscapeMap escapeCharacters{
            {'a', "escape sequence"}};
    StringFormatter::Map actions;
    StringFormatter stringFormatterUnderTest{actions, '%', ':', '\\',
            escapeCharacters};

    BOOST_CHECK_THROW(stringFormatterUnderTest.formatString("\\"),
            UnterminatedEscapeSequence);
}


BOOST_AUTO_TEST_SUITE_END()

