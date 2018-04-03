#pragma once

#include <set>

struct TableRow
{
	int mId;
	std::string mname;

	friend bool operator < (const TableRow& left, const TableRow& right)
	{
		return left.mId < right.mId;
	}
};

using TableIndex = std::set<TableRow>;

enum class OperationStatus
{
	Ok,
	NoTable,
	DuplicateRecord
};

struct TableManagerStatus
{
	TableManagerStatus(aStatus = OperationStatus::Ok)
		: mStatus(aStatus)
	{
	}
	OperationStatus mStatus;
	std::string mMessage;
};

class TableManager
{
public:
	TableManager();

	TableManagerStatus Insert(const std::string& aTableName, const TableRow& aRow);
	TableManagerStatus Truncate(const std::string& aTableName);

	TableManagerStatus Intersection();
	TableManagerStatus SymmetricDifference();
	
private:
	std::unordered_map<std::string, TableIndex> mTables;
}