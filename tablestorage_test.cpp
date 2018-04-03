#define BOOST_TEST_MODULE tablestorage_test

#include <boost/test/unit_test.hpp>

#include "tablestorage.h"

BOOST_AUTO_TEST_SUITE(tablestorage_test)

struct Fixture
{
    Fixture()
    {
    }

    TableStorage mTableStorage;
};

BOOST_FIXTURE_TEST_CASE(test_insert, Fixture)
{
    mTableStorage.Insert("A", 0, "lean");
    mTableStorage.Insert("A", 1, "sweater");
    mTableStorage.Insert("A", 2, "frank");
    mTableStorage.Insert("A", 3, "violation");
    mTableStorage.Insert("A", 4, "quality");
    mTableStorage.Insert("A", 5, "precision");

    const char* output = "0,lean\n1,sweater\n2,frank\n3,violation\n4,quality\n5,precision";
    BOOST_CHECK_EQUAL(mTableStorage.Dump(), output);
}

BOOST_FIXTURE_TEST_CASE(test_truncate, Fixture)
{
    mTableStorage.Insert("A", 0, "lean");
    mTableStorage.Insert("A", 1, "sweater");
    mTableStorage.Insert("A", 2, "frank");
    mTableStorage.Insert("A", 3, "violation");
    mTableStorage.Insert("A", 4, "quality");
    mTableStorage.Insert("A", 5, "precision");

    mTableStorage.Truncate();

    const char* output = "";
    BOOST_CHECK_EQUAL(mTableStorage.Dump(), output);
}

BOOST_FIXTURE_TEST_CASE(test_intersect, Fixture)
{
    mTableStorage.Insert("A", 0, "lean");
    mTableStorage.Insert("A", 1, "sweater");
    mTableStorage.Insert("A", 2, "frank");
    mTableStorage.Insert("A", 3, "violation");
    mTableStorage.Insert("A", 4, "quality");
    mTableStorage.Insert("A", 5, "precision");

    mTableStorage.Insert("B", 3, "proposal");
    mTableStorage.Insert("B", 4, "example");
    mTableStorage.Insert("B", 5, "lake");
    mTableStorage.Insert("B", 6, "flour");
    mTableStorage.Insert("B", 7, "wonder");
    mTableStorage.Insert("B", 8, "selection");

    const char* output = "3,violation,proposal\n4,quality,example\n5,precision,lake";
    BOOST_CHECK_EQUAL(mTableStorage.Intersect(), output);
}

BOOST_FIXTURE_TEST_CASE(test_symmetric_difference, Fixture)
{
    mTableStorage.Insert("A", 0, "lean");
    mTableStorage.Insert("A", 1, "sweater");
    mTableStorage.Insert("A", 2, "frank");
    mTableStorage.Insert("A", 3, "violation");
    mTableStorage.Insert("A", 4, "quality");
    mTableStorage.Insert("A", 5, "precision");

    mTableStorage.Insert("B", 3, "proposal");
    mTableStorage.Insert("B", 4, "example");
    mTableStorage.Insert("B", 5, "lake");
    mTableStorage.Insert("B", 6, "flour");
    mTableStorage.Insert("B", 7, "wonder");
    mTableStorage.Insert("B", 8, "selection");

    const char* output = "0,lean,\n1,sweater,\n2,frank,\n6,,flour\n7,,wonder\n8,,selection";
    BOOST_CHECK_EQUAL(mTableStorage.SymmetricDifference(), output);
}

}
