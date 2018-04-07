#include <assert.h>
#include <sstream>

#include "tablestorage.h"

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

    TablesLock lock(mTableIter);

    for (std::size_t i = 0; i < mTableCount; ++i)
    {
        if (mTableIter[i]->second.mIndex.size() < maxSize)
            maxSizeIndex = i;
    }

    std::stringstream str;

    for (const auto& p : mTableIter[maxSizeIndex]->second.mIndex)
        FindAndPrintIfFound(mTableIter[mTableCount - maxSizeIndex - 1]->second.mIndex, p, str, maxSizeIndex);

    return CompleteOperationStatus{OperationStatus::Ok, str.str()};
}

CompleteOperationStatus TableManager::SymmetricDifference()
{
    std::stringstream str;

    TablesLock lock(mTableIter);

    for (std::size_t i = 0; i < mTableCount; ++i)
    {
        for (const auto& p : mTableIter[i]->second.mIndex)
            FindAndPrintIfNotFound(mTableIter[mTableCount - i - 1]->second.mIndex, p, str, i);
    }
    return CompleteOperationStatus{OperationStatus::Ok, str.str()};
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
