#pragma once

#include "itablemanager.h"
#include "structs.h"

class CommandExecutor
{
public:
    CommandExecutor(ITableManager* aTableManager);

    CompleteOperationStatus RunCommand(const std::string& aLine);

private:
    CompleteCommand Parse(const std::string& aLine);

    CompleteCommand HandleInsert(const std::string& aLine);
    CompleteCommand HandleTruncate(const std::string& aLine);
    CompleteCommand HandleIntersection(const std::string& aLine);
    CompleteCommand HandleSymmetricDifference(const std::string& aLine);

    ITableManager* mTableManager;

    static constexpr const char* CommandInsert = "INSERT";
    static constexpr const char* CommandTruncate = "TRUNCATE";
    static constexpr const char* CommandIntersection = "INTERSECTION";
    static constexpr const char* CommandSymmetricDifference = "SYMMETRIC_DIFFERENCE";
};
