#include "DefaultValue.hpp"

#include <boost/program_options.hpp>
#include <boost/test/unit_test.hpp>

namespace po = boost::program_options;

BOOST_AUTO_TEST_SUITE(DefaultValueTest)

constexpr int defaultValue = 2364;

struct Fixture {
    int value = defaultValue;
    po::options_description options;
    po::variables_map variablesMap;

    Fixture() {
        options.add_options()("value", util::defaultValue(value));
    }

    void run(const std::vector<std::string>& arguments) {
        po::store(po::command_line_parser{arguments}.options(options)
                .run(), variablesMap);
        po::notify(variablesMap);
    }
};

BOOST_FIXTURE_TEST_CASE(DefaultValueUsed, Fixture) {
    run({});
    BOOST_TEST(value == defaultValue);
}

BOOST_FIXTURE_TEST_CASE(DefaultValueOverridden, Fixture) {
    run({"--value=21"});
    BOOST_TEST(value == 21);
}

BOOST_AUTO_TEST_SUITE_END() // DefaultValueTest
