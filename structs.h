#pragma once

#include <string>

struct TableRow
{
    int mId;
    std::string mName;

    TableRow(int aId, const std::string& aName = "")
        : mId(aId)
        , mName(aName)
    {
    }

    friend bool operator < (const TableRow& left, const TableRow& right)
    {
        return left.mId < right.mId;
    }
};

enum class OperationStatus
{
    Ok,
    UnknownCommand,
    NoTable,
    DuplicateRecord
};

struct CompleteOperationStatus
{
    CompleteOperationStatus(OperationStatus aStatus = OperationStatus::Ok, const std::string& aMessage = "")
        : mStatus(aStatus)
        , mMessage(aMessage)
    {
    }
    OperationStatus mStatus;
    std::string mMessage;

    friend bool operator == (const CompleteOperationStatus& left, const CompleteOperationStatus& right)
    {
        return left.mStatus == right.mStatus && left.mMessage == right.mMessage;
    }
};

enum class Command
{
    Insert,
    Truncate,
    Intersect,
    SymmetricDifference,
};

struct CompleteCommand
{
    Command mCommand;
    TableRow mRow;
};
