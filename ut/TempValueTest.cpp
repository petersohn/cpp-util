#include "util/TempValue.hpp"
#include <boost/test/unit_test.hpp>

using namespace util;

BOOST_AUTO_TEST_SUITE(TempValueTest)

BOOST_AUTO_TEST_CASE(change_value_and_change_back)
{
	const int originalValue = 12;
	const int newValue = 65;

	int value = originalValue;

	{
		TempValue<int> tmp{value, newValue};
		BOOST_CHECK_EQUAL(value, newValue);
	}
	BOOST_CHECK_EQUAL(value, originalValue);
}

BOOST_AUTO_TEST_SUITE_END()



