#pragma once

#include <set>

using TableIndex = std::set<TableRow>;

class TableManager : public ITableManager
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