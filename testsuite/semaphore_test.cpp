#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "semaphore.hpp"

#include <chrono>
#include <future>
#include <thread>

using namespace nova;
using namespace boost;

BOOST_AUTO_TEST_CASE( sem_timed_wait )
{
    timed_semaphore sem;

    auto now = std::chrono::high_resolution_clock::now().time_since_epoch() + std::chrono::milliseconds(500);

    auto seconds     = std::chrono::duration_cast<std::chrono::seconds>(now);
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(seconds - now);

    struct timespec timeoutspec;
    timeoutspec.tv_sec  = seconds.count();
    timeoutspec.tv_nsec = nanoseconds.count();

    int status = sem.timed_wait(timeoutspec);
    BOOST_REQUIRE(!status);
}

namespace {

const int thread_count = 8;
const int iterations_per_thread = 1000000;

int count = 0;

semaphore s(1);

void test_fn(void)
{
    for (int i = 0; i != iterations_per_thread; ++i) {
        s.wait();
        ++count;
        s.post();
    }
}

}

BOOST_AUTO_TEST_CASE( sem_test )
{
    {
        auto a0 = std::async(std::launch::async, test_fn);
        auto a1 = std::async(std::launch::async, test_fn);
        auto a2 = std::async(std::launch::async, test_fn);
        auto a3 = std::async(std::launch::async, test_fn);
        auto a4 = std::async(std::launch::async, test_fn);
        auto a5 = std::async(std::launch::async, test_fn);
        auto a6 = std::async(std::launch::async, test_fn);
        auto a7 = std::async(std::launch::async, test_fn);
    }

    BOOST_REQUIRE_EQUAL(count, iterations_per_thread * thread_count);
}

BOOST_AUTO_TEST_CASE( sem_sync_test )
{
    semaphore sem(0);
    sem.post();
    semaphore_sync<semaphore> sync(sem);
}

BOOST_AUTO_TEST_CASE( sem_try_wait_test )
{
    semaphore sem(0);

    BOOST_REQUIRE(!sem.try_wait());
    sem.post();
    BOOST_REQUIRE(sem.try_wait());
}
