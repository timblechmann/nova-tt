#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <atomic>

#include "thread_pool.hpp"

BOOST_AUTO_TEST_CASE( int_test )
{
    std::atomic_int count(0);

    {
        nova::thread_pool pool(4);

        for (int i = 0; i != 100; ++i) {
            pool.schedule([&]() {
                count += 1;
            });
        }
    }

    BOOST_REQUIRE_EQUAL( count, 100 );
}

BOOST_AUTO_TEST_CASE( future_test )
{
    std::future<int> answer;

    {
        nova::thread_pool pool(4);

        answer = pool.schedule([]() {
            return 42;
        });
    }

    BOOST_REQUIRE_EQUAL( answer.get(), 42 );
}
