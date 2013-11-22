//#define BOOST_TEST_MAIN 1
#define BOOST_TEST_DYN_LINK 1
#include  <boost/test/unit_test.hpp>
#include <iostream>

#ifdef BOOST_TEST_ALTERNATIVE_INIT_API
bool init_unit_test_suite()
{
    return true;
}
#else
boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] ){
return NULL;
}
#endif

int BOOST_TEST_CALL_DECL
main( int argc, char* argv[] )
{
    time_t random_seed = time(0);
    if (argc == 2)
    {
        random_seed = atol(argv[1]);
    }
    std::cout << "Random seed: " << random_seed << std::endl;
    srand(random_seed);

    return ::boost::unit_test::unit_test_main( init_unit_test_suite, argc, argv );
}

