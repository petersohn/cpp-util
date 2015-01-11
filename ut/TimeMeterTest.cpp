#include "util/TimeMeter.hpp"
#include <boost/test/unit_test.hpp>

using namespace util;

BOOST_AUTO_TEST_SUITE(TimeMeterTest)

BOOST_AUTO_TEST_CASE(compile)
{
	TimeMeter t;
	auto processorTime = t.processorTime();
	auto realTime = t.realTime();
	auto data = t.data();
	(void)processorTime;
	(void)realTime;
	(void)data;
}

BOOST_AUTO_TEST_SUITE_END()


