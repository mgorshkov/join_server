#pragma once

struct TableRow
{
    int mId;
    std::string mname;

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
    CompleteOperationStatus(aStatus = OperationStatus::Ok)
        : mStatus(aStatus)
    {
    }
    OperationStatus mStatus;
    std::string mMessage;
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
