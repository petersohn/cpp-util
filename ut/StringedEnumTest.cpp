#include <boost/test/unit_test.hpp>
#include <sstream>
#include "StringedEnum.hpp"

BOOST_AUTO_TEST_SUITE(StringedEnumTest)

STRINGED_ENUM(Number, (zero)(one)(two))

BOOST_AUTO_TEST_CASE(STRINGED_ENUM_shoud_instantiate_enum) {
	BOOST_CHECK_EQUAL(static_cast<int>(Number::zero), 0);
	BOOST_CHECK_EQUAL(static_cast<int>(Number::one), 1);
	BOOST_CHECK_EQUAL(static_cast<int>(Number::two), 2);
}

BOOST_AUTO_TEST_CASE(STRINGED_ENUM_shoud_create_output_function) {
	{
		std::ostringstream ss;
		ss << Number::zero;
		BOOST_CHECK_EQUAL(ss.str(), "zero");
	}
	{
		std::ostringstream ss;
		ss << Number::one;
		BOOST_CHECK_EQUAL(ss.str(), "one");
	}
	{
		std::ostringstream ss;
		ss << Number::two;
		BOOST_CHECK_EQUAL(ss.str(), "two");
	}
}

BOOST_AUTO_TEST_SUITE_END()

