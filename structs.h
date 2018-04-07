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

    friend std::ostream& operator << (std::ostream& stream, const TableRow& tr)
    {
        stream << std::to_string(tr.mId);
        stream << tr.mName;
        return stream;
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
            stream << std::string(" ") << os.mMessage;
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

inline std::ostream& operator << (std::ostream& stream, Command c)
{
    std::string status;
    switch (c)
    {
    case Command::Insert:
        status = "Insert";
        break;
    case Command::Truncate:
        status = "Truncate";
        break;
    case Command::Intersection:
        status = "Intersection";
        break;
    case Command::SymmetricDifference:
        status = "SymmetricDifference";
        break;
    case Command::Error:
        status = "Error";
        break;
    default:
        status = "ERR unknown command";
        break;
    }
    return stream << status;
}

struct CompleteCommand
{
    Command mCommand;
    std::string mTableName;
    TableRow mRow;

    friend std::ostream& operator << (std::ostream& stream, CompleteCommand cc)
    {
        stream << cc.mCommand;
        stream << cc.mTableName;
        stream << cc.mRow;
        return stream;
    }
};
