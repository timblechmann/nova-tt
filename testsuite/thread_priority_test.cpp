#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <boost/tuple/tuple.hpp>

#include <iostream>

#include "thread_priority.hpp"

using namespace nova;
using namespace std;

BOOST_AUTO_TEST_CASE( priority_test )
{
    int current_priority = thread_priority();
    int low, high;
    boost::tie(low, high) = thread_priority_interval();

    int low_rt, high_rt;
    boost::tie(low_rt, high_rt) = thread_priority_interval_rt();
    BOOST_CHECK(thread_set_priority_rt(low_rt));
}
