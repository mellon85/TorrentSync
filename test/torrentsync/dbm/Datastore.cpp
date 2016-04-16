#include <boost/test/unit_test.hpp>
#include <torrentsync/dbm/Datastore.h>
#include <boost/filesystem.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(torrentsync_dbm_Datastore);

using namespace torrentsync::dbm;
namespace fs = boost::filesystem;

BOOST_AUTO_TEST_CASE(create_dbm) {
    auto path = fs::unique_path();
    BOOST_REQUIRE(! fs::exists(path.c_str()));
    {
        BOOST_REQUIRE_NO_THROW(
            Datastore db(path));
        BOOST_REQUIRE(fs::exists(path));
    }
    BOOST_REQUIRE(fs::exists(path));
    fs::remove_all(path);
}

BOOST_AUTO_TEST_CASE(empty_transaction) {
    auto path = fs::unique_path();
    BOOST_REQUIRE(! fs::exists(path.c_str()));
    {
        Datastore db(path);
        auto schema = db.getTransaction();
        BOOST_REQUIRE(fs::exists(path));
    }
    BOOST_REQUIRE(fs::exists(path));
    fs::remove_all(path);
}

BOOST_AUTO_TEST_SUITE_END();

