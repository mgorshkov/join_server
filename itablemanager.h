#pragma once

#include <string>
#include "structs.h"

class ITableManager
{
public:
    virtual ~ITableManager() = default;

    virtual CompleteOperationStatus Insert(const std::string& aTableName, const TableRow& aRow) = 0;
    virtual CompleteOperationStatus Truncate(const std::string& aTableName) = 0;

    virtual CompleteOperationStatus Intersection() = 0;
    virtual CompleteOperationStatus SymmetricDifference() = 0;
};