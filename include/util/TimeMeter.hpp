#ifndef INCLUDE_UTIL_TIMEMETER_HPP
#define INCLUDE_UTIL_TIMEMETER_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <ctime>

namespace util {

struct TimerData {
    boost::posix_time::time_duration processorTime;
    boost::posix_time::time_duration realTime;
};

class TimeMeter {

    std::clock_t beginProcessorTime;
    boost::posix_time::ptime beginRealTime;

public:

    TimeMeter():
        beginProcessorTime(std::clock()),
        beginRealTime(boost::posix_time::microsec_clock::universal_time())
    {}

    void reset()
    {
        beginProcessorTime = std::clock();
        beginRealTime = boost::posix_time::microsec_clock::universal_time();
    }

    boost::posix_time::time_duration processorTime() const
    {
        std::clock_t value = std::clock() - beginProcessorTime;
        return boost::posix_time::millisec(value / (CLOCKS_PER_SEC / 1000));
    }

    boost::posix_time::time_duration realTime() const
    {
        return boost::posix_time::microsec_clock::universal_time() - beginRealTime;
    }

    TimerData data() const
    {
        return {processorTime(), realTime()};
    }

};

}


#endif /* INCLUDE_UTIL_TIMEMETER_HPP */
