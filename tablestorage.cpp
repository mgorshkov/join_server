#include <assert.h>
#include <sstream>

#include "tablestorage.h"

const char* TableManager::TableNames[TableManager::TableCount] = { "A", "B" };

CompleteOperationStatus TableManager::Create(const std::string& aTableName)
{
    auto tablesIt = mTables.find(aTableName);
    if (tablesIt != mTables.end())
        return CompleteOperationStatus{OperationStatus::TableAlreadyExists};
    mTables.insert(std::make_pair(aTableName, TableIndex{}));
    return CompleteOperationStatus{};
}

CompleteOperationStatus TableManager::Insert(const std::string& aTableName, const TableRow& aRow)
{
    auto tablesIt = mTables.find(aTableName);
    if (tablesIt == mTables.end())
    {
        auto createRes = Create(aTableName);
        if (createRes.mStatus != OperationStatus::Ok)
            return createRes;
        tablesIt = mTables.find(aTableName);
    }
    auto result = tablesIt->second.emplace(aRow);
    if (!result.second)
        return CompleteOperationStatus{OperationStatus::DuplicateRecord};
    return CompleteOperationStatus{};
}

CompleteOperationStatus TableManager::Truncate(const std::string& aTableName)
{
    auto tablesIt = mTables.find(aTableName);
    if (tablesIt == mTables.end())
        return CompleteOperationStatus{OperationStatus::NoTable};
    tablesIt->second.clear();
    return CompleteOperationStatus{};
}

void TableManager::FindAndPrintIfFound(
    const TableIndex& aIndex,
    const TableRow& aRow,
    std::ostream& aStr,
    std::size_t aTableNumber)
{
    auto it = aIndex.find(TableRow{aRow.mId});
    if (it != aIndex.end())
        aStr
            << aRow.mId
            << ","
            << (aTableNumber == 0 ? aRow.mName : it->mName)
            << ","
            << (aTableNumber == 0 ? it->mName : aRow.mName)
            << std::endl;
}

void TableManager::FindAndPrintIfNotFound(
    const TableIndex& aIndex,
    const TableRow& aRow,
    std::ostream& aStr,
    std::size_t aTableNumber)
{
    auto it = aIndex.find(TableRow{aRow.mId});
    if (it == aIndex.end())
        aStr
            << aRow.mId
            << ","
            << (aTableNumber == 0 ? "" : ",")
            << aRow.mName
            << (aTableNumber != 0 ? "" : ",")
            << std::endl;
}

CompleteOperationStatus TableManager::Intersection()
{
    std::size_t maxSizeIndex = -1;
    std::size_t maxSize = static_cast<std::size_t>(-1);
    TableMap::const_iterator it[TableCount];
    for (std::size_t i = 0; i < TableCount; ++i)
    {
        it[i] = mTables.find(TableNames[i]);
        assert (it[i] != mTables.end());

        if (it[i]->second.size() < maxSize)
            maxSizeIndex = i;
    }

    std::stringstream str;

    for (const auto& p : it[maxSizeIndex]->second)
        FindAndPrintIfFound(it[TableCount - maxSizeIndex - 1]->second, p, str, maxSizeIndex);

    return CompleteOperationStatus{OperationStatus::Ok, str.str()};
}

CompleteOperationStatus TableManager::SymmetricDifference()
{
    std::stringstream str;

    TableMap::const_iterator it[TableCount];
    for (std::size_t i = 0; i < TableCount; ++i)
    {
        it[i] = mTables.find(TableNames[i]);
        assert (it[i] != mTables.end());
    }
    for (std::size_t i = 0; i < TableCount; ++i)
    {
        for (const auto& p : it[i]->second)
            FindAndPrintIfNotFound(it[TableCount - i - 1]->second, p, str, i);
    }
    return CompleteOperationStatus{OperationStatus::Ok, str.str()};
}

std::string TableManager::Dump()
{
    std::stringstream str;

    for (const auto& tableName : TableNames)
    {
        auto it = mTables.find(tableName);
        if (it != mTables.end())
        {
            str << tableName << ":" << std::endl;
            for (const auto& p : it->second)
                str << p.mId << "," << p.mName << std::endl;
        }
    }

    return str.str();
}
