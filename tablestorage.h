#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "structs.h"
#include "itablemanager.h"

class TableManager : public ITableManager
{
public:
	TableManager();

    CompleteOperationStatus Insert(const std::string& aTableName, const TableRow& aRow);
    CompleteOperationStatus Truncate(const std::string& aTableName);

    CompleteOperationStatus Intersection();
    CompleteOperationStatus SymmetricDifference();

    std::string Dump();

private:
    CompleteOperationStatus Create(const std::string& aTableName);
    void FindAndPrintIfFound(
        const TableIndex& aIndex,
        const TableRow& aRow,
        std::ostream& aStr,
        std::size_t aTableNumber);
    void FindAndPrintIfNotFound(
        const TableIndex& aIndex,
        const TableRow& aRow,
        std::ostream& aStr,
        std::size_t aTableNumber);

    Tables mTables;

    const std::set<std::string> mTableNames;
    const std::size_t mTableCount;
    std::vector<Tables::iterator> mTableIter;

    std::size_t mCurTableNumber;

    class TablesLock
    {
    public:
    	TablesLock(std::vector<Tables::iterator>& aTableIter)
    	{
    		for (auto& it : aTableIter)
    			mGuards.emplace_back(it->second.mMutex);
    	}

    private:
    	std::vector<std::unique_lock<std::mutex>> mGuards;
    };
};
