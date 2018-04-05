#pragma once

#include <string>

struct TableRow
{
    int mId;
    std::string mName;

    TableRow(int aId = -1, const std::string& aName = "")
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
    TableAlreadyExists,
    DuplicateRecord
};

inline std::ostream& operator << (std::ostream& stream, OperationStatus os)
{
    std::string status;
    switch (os)
    {
    case OperationStatus::Ok:
        status = "Ok";
        break;
    case OperationStatus::UnknownCommand:
        status = "UnknownCommand";
        break;
    case OperationStatus::NoTable:
        status = "NoTable";
        break;
    case OperationStatus::TableAlreadyExists:
        status = "TableAlreadyExists";
        break;
    case OperationStatus::DuplicateRecord:
        status = "DuplicateRecord";
        break;
    default:
        status = "Unknown";
        break;
    }
    return stream << status;
}

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

    friend std::ostream& operator << (std::ostream& stream, CompleteOperationStatus os)
    {
        stream << os.mStatus;
        stream << os.mMessage;
        return stream;
    }
};

enum class Command
{
    Insert,
    Truncate,
    Intersection,
    SymmetricDifference,
    Error,
};

struct CompleteCommand
{
    Command mCommand;
    std::string mTableName;
    TableRow mRow;
};