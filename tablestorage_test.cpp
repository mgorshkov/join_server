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
    BOOST_CHECK_EQUAL(result, ok);
    result = mTableManager.Insert("A", TableRow{1, "sweater"});
    BOOST_CHECK_EQUAL(result, ok);
    result = mTableManager.Insert("A", TableRow{2, "frank"});
    BOOST_CHECK_EQUAL(result, ok);
    result = mTableManager.Insert("A", TableRow{3, "violation"});
    BOOST_CHECK_EQUAL(result, ok);
    result = mTableManager.Insert("A", TableRow{4, "quality"});
    BOOST_CHECK_EQUAL(result, ok);
    result = mTableManager.Insert("A", TableRow{5, "precision"});
    BOOST_CHECK_EQUAL(result, ok);

    std::stringstream str;
    str << "A:" << std::endl;
    str << "0,lean" << std::endl;
    str << "1,sweater" << std::endl;
    str << "2,frank" << std::endl;
    str << "3,violation" << std::endl;
    str << "4,quality" << std::endl;
    str << "5,precision" << std::endl;
    
    BOOST_CHECK_EQUAL(mTableManager.Dump(), str.str());
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

    std::stringstream str;
    str << "A:" << std::endl;

    BOOST_CHECK_EQUAL(mTableManager.Dump(), str.str());
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

    std::stringstream str;
    str << "3,violation,proposal" << std::endl;
    str << "4,quality,example" << std::endl;
    str << "5,precision,lake" << std::endl;

    BOOST_CHECK_EQUAL(result.mMessage, str.str());
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
    BOOST_CHECK_EQUAL(result.mStatus, OperationStatus::Ok);

    std::stringstream str;
    str << "0,lean," << std::endl;
    str << "1,sweater," << std::endl;
    str << "2,frank," << std::endl;
    str << "6,,flour" << std::endl;
    str << "7,,wonder" << std::endl;
    str << "8,,selection" << std::endl;

    BOOST_CHECK_EQUAL(result.mMessage, str.str());
}

}
