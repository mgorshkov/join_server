#pragma once

#include <set>
#include <unordered_map>

#include "structs.h"
#include "itablemanager.h"

using TableIndex = std::set<TableRow>;
using TableMap = std::unordered_map<std::string, TableIndex>;

class TableManager : public ITableManager
{
public:
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
    TableMap mTables;
    static const size_t TableCount = 2;
    static const char* TableNames[TableCount];
};
