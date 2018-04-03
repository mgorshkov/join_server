#pragma once

struct TableRow
{
	int id;
	std::string name;
};

using TableIndex = std::map<int, TableRow>;

enum class TableManagerStatus
{
	Ok,
	NoTable,
	DuplicateRecord
};

class TableManager
{
public:
	TableManager()
	{
		mTables["A"] = TableIndex();
		mTables["B"] = TableIndex();
	}

	TableManagerStatus Insert(const std::string& aTableName, int aId, const std::string& aName)
	{
		auto tablesIt = mTables.find(aTableName);
		if (tablesIt == mTables.end())
			return TableManagerStatus::NoTable;
		auto recordIt = tablesIt->find(aId);
		if (recordIt != tablesIt->end())
			return TableManagerStatus::DuplicateRecord;
		tablesIt->insert(std::pair(aId, aName));
		return TableManagerStatus::Ok;
	}

	TableManagerStatus Truncate(const std::string& aTableName)
	{
		auto tablesIt = mTables.find(aTableName);
		if (tablesIt == mTables.end())
			return TableManagerStatus::NoTable;
		tablesIt->clear();
		return TableManagerStatus::Ok;
	}
	
private:
	std::unordered_map<std::string, TableIndex> mTables;
}