#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "physical_concurrency.hpp"

using namespace nova;

BOOST_AUTO_TEST_CASE( physical_concurrency_test )
{
	std::cout << "Physical Concurrency: " << physical_concurrency() << std::endl;
}

