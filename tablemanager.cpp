#include <assert.h>
#include <iostream>
#include <sstream>

#include "tablemanager.h"

TableManager::TableManager()
    : mTableNames{"A", "B"}
    , mTableCount(mTableNames.size())
    , mTableIter(mTableNames.size())
    , mCurTableNumber(0)
{
    for (const auto& tableName : mTableNames)
        Create(tableName);
}

CompleteOperationStatus TableManager::Create(const std::string& aTableName)
{
    auto tableNamesIt = mTableNames.find(aTableName);
    if (tableNamesIt == mTableNames.end())
        return CompleteOperationStatus{OperationStatus::UnknownTableName};

    auto tablesIt = mTables.find(aTableName);
    if (tablesIt != mTables.end())
        return CompleteOperationStatus{OperationStatus::TableAlreadyExists};
    bool result;
    std::tie(mTableIter[mCurTableNumber], result) = mTables.emplace(std::make_pair(aTableName, mCurTableNumber));
    ++mCurTableNumber;
    return CompleteOperationStatus{};
}

CompleteOperationStatus TableManager::Insert(const std::string& aTableName, const TableRow& aRow)
{
    auto tablesIt = mTables.find(aTableName);
    if (tablesIt == mTables.end())
        return CompleteOperationStatus{OperationStatus::UnknownTableName};

    std::lock_guard<std::mutex> lock(tablesIt->second.mMutex);

    auto result = tablesIt->second.mIndex.emplace(aRow);
    if (!result.second)
        return CompleteOperationStatus{OperationStatus::DuplicateRecord, std::to_string(aRow.mId)};
    return CompleteOperationStatus{};
}

CompleteOperationStatus TableManager::Truncate(const std::string& aTableName)
{
    auto tablesIt = mTables.find(aTableName);
    if (tablesIt == mTables.end())
        return CompleteOperationStatus{OperationStatus::NoTable};

    std::lock_guard<std::mutex> lock(tablesIt->second.mMutex);

    tablesIt->second.mIndex.clear();
    return CompleteOperationStatus{};
}

std::string TableManager::FindAndPrintIfFound(
    const TableIndex& aIndex,
    const TableRow& aRow,
    std::size_t aTableNumber)
{
    std::stringstream str;
    auto it = aIndex.find(TableRow{aRow.mId});
    if (it != aIndex.end())
        str
            << aRow.mId
            << ","
            << (aTableNumber == 0 ? aRow.mName : it->mName)
            << ","
            << (aTableNumber == 0 ? it->mName : aRow.mName)
            << std::endl;
    return str.str();
}

std::string TableManager::FindAndPrintIfNotFound(
    const TableIndex& aIndex,
    const TableRow& aRow,
    std::size_t aTableNumber)
{
    std::stringstream str;
    auto it = aIndex.find(TableRow{aRow.mId});
    if (it == aIndex.end())
        str
            << aRow.mId
            << ","
            << (aTableNumber == 0 ? "" : ",")
            << aRow.mName
            << (aTableNumber != 0 ? "" : ",")
            << std::endl;
    return str.str();
}

CompleteOperationStatus TableManager::Intersection()
{
    std::size_t maxSizeIndex = -1;
    std::size_t maxSize = static_cast<std::size_t>(-1);

    TablesLock lock(mTableIter);

    for (std::size_t i = 0; i < mTableCount; ++i)
    {
        if (mTableIter[i]->second.mIndex.size() < maxSize)
            maxSizeIndex = i;
    }

    std::string str;

    for (const auto& p : mTableIter[maxSizeIndex]->second.mIndex)
        str += FindAndPrintIfFound(mTableIter[mTableCount - maxSizeIndex - 1]->second.mIndex, p, maxSizeIndex);
#ifdef DEBUG_PRINT
    std::cout << "TableManager::Intersection, this==" << this << ", str=" << str << std::endl;
#endif
    return CompleteOperationStatus{OperationStatus::Ok, "", str};
}

CompleteOperationStatus TableManager::SymmetricDifference()
{
    std::string str;

    TablesLock lock(mTableIter);

    for (std::size_t i = 0; i < mTableCount; ++i)
    {
        for (const auto& p : mTableIter[i]->second.mIndex)
            str += FindAndPrintIfNotFound(mTableIter[mTableCount - i - 1]->second.mIndex, p, i);
    }
    return CompleteOperationStatus{OperationStatus::Ok, "", str};
}

std::string TableManager::Dump()
{
    std::stringstream str;

    TablesLock lock(mTableIter);

    for (const auto& table : mTables)
    {
    	if (table.second.mIndex.empty())
            continue;
        str << table.first << ":" << std::endl;
        for (const auto& p : table.second.mIndex)
            str << p.mId << "," << p.mName << std::endl;
    }

    return str.str();
}
