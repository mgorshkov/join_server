#define BOOST_TEST_MODULE tablestorage_test

#include <boost/test/unit_test.hpp>

#include "tablestorage.h"

BOOST_AUTO_TEST_SUITE(tablestorage_test)

struct Fixture
{
    Fixture()
    {
    }

    TableManager mTableManager;
};

BOOST_FIXTURE_TEST_CASE(test_insert, Fixture)
{
    CompleteOperationStatus ok{OperationStatus::Ok, ""};
    auto result = mTableManager.Insert("A", TableRow{0, "lean"});
    BOOST_CHECK(result == ok);
    result = mTableManager.Insert("A", TableRow{1, "sweater"});
    BOOST_CHECK(result == ok);
    result = mTableManager.Insert("A", TableRow{2, "frank"});
    BOOST_CHECK(result == ok);
    result = mTableManager.Insert("A", TableRow{3, "violation"});
    BOOST_CHECK(result == ok);
    result = mTableManager.Insert("A", TableRow{4, "quality"});
    BOOST_CHECK(result == ok);
    result = mTableManager.Insert("A", TableRow{5, "precision"});
    BOOST_CHECK(result == ok);

    const char* output = "0,lean\n1,sweater\n2,frank\n3,violation\n4,quality\n5,precision";
    BOOST_CHECK_EQUAL(mTableManager.Dump(), std::string(output));
}

BOOST_FIXTURE_TEST_CASE(test_truncate, Fixture)
{
    mTableManager.Insert("A", TableRow{0, "lean"});
    mTableManager.Insert("A", TableRow{1, "sweater"});
    mTableManager.Insert("A", TableRow{2, "frank"});
    mTableManager.Insert("A", TableRow{3, "violation"});
    mTableManager.Insert("A", TableRow{4, "quality"});
    mTableManager.Insert("A", TableRow{5, "precision"});

    mTableManager.Truncate("A");

    const char* output = "";
    BOOST_CHECK_EQUAL(mTableManager.Dump(), output);
}

BOOST_FIXTURE_TEST_CASE(test_intersection, Fixture)
{
    mTableManager.Insert("A", TableRow{0, "lean"});
    mTableManager.Insert("A", TableRow{1, "sweater"});
    mTableManager.Insert("A", TableRow{2, "frank"});
    mTableManager.Insert("A", TableRow{3, "violation"});
    mTableManager.Insert("A", TableRow{4, "quality"});
    mTableManager.Insert("A", TableRow{5, "precision"});

    mTableManager.Insert("B", TableRow{3, "proposal"});
    mTableManager.Insert("B", TableRow{4, "example"});
    mTableManager.Insert("B", TableRow{5, "lake"});
    mTableManager.Insert("B", TableRow{6, "flour"});
    mTableManager.Insert("B", TableRow{7, "wonder"});
    mTableManager.Insert("B", TableRow{8, "selection"});

    auto result = mTableManager.Intersection();
    BOOST_CHECK_EQUAL(result.mStatus, OperationStatus::Ok);
    BOOST_CHECK_EQUAL(result.mMessage, "3,violation,proposal\n4,quality,example\n5,precision,lake");
}

BOOST_FIXTURE_TEST_CASE(test_symmetric_difference, Fixture)
{
    mTableManager.Insert("A", TableRow{0, "lean"});
    mTableManager.Insert("A", TableRow{1, "sweater"});
    mTableManager.Insert("A", TableRow{2, "frank"});
    mTableManager.Insert("A", TableRow{3, "violation"});
    mTableManager.Insert("A", TableRow{4, "quality"});
    mTableManager.Insert("A", TableRow{5, "precision"});

    mTableManager.Insert("B", TableRow{3, "proposal"});
    mTableManager.Insert("B", TableRow{4, "example"});
    mTableManager.Insert("B", TableRow{5, "lake"});
    mTableManager.Insert("B", TableRow{6, "flour"});
    mTableManager.Insert("B", TableRow{7, "wonder"});
    mTableManager.Insert("B", TableRow{8, "selection"});

    auto result = mTableManager.SymmetricDifference();
    //BOOST_CHECK_EQUAL(result.mStatus, OperationStatus::Ok);
    //BOOST_CHECK_EQUAL(result.mMessage, "0,lean,\n1,sweater,\n2,frank,\n6,,flour\n7,,wonder\n8,,selection");
}

}
