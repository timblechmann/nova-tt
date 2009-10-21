#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "semaphore.hpp"

#include <boost/thread/thread.hpp>

using namespace nova;
using namespace boost;

namespace
{
const int thread_count = 8;

int count = 0;

semaphore s(1);

void test_fn(void)
{
    for (int i = 0; i != 1000000; ++i)
    {
        s.wait();
        ++count;
        s.post();
    }
}

}

BOOST_AUTO_TEST_CASE( sem_test )
{
    thread_group g;

    for (int i = 0; i != thread_count; ++i)
        g.create_thread(test_fn);
    g.join_all();

    BOOST_REQUIRE_EQUAL(count, 8000000);
}
