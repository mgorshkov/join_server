#pragma once

#include <mutex>
#include <set>
#include <string>
#include <unordered_map>

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

using TableIndex = std::set<TableRow>;

struct Table
{
    size_t mNumber;
    TableIndex mIndex;
    std::mutex mMutex;

    Table(size_t aNumber)
        : mNumber(aNumber)
    {        
    }
};

using Tables = std::unordered_map<std::string, Table>;

enum class OperationStatus
{
    Ok,
    UnknownTableName,
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
        status = "OK";
        break;
    case OperationStatus::UnknownCommand:
        status = "ERR unknown command";
        break;
    case OperationStatus::NoTable:
        status = "ERR no table";
        break;
    case OperationStatus::TableAlreadyExists:
        status = "ERR table already exists";
        break;
    case OperationStatus::DuplicateRecord:
        status = "ERR duplicate";
        break;
    default:
        status = "ERR unknown status";
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
        if (!os.mMessage.empty())
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
