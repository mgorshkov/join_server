#pragma once

#include <set>
#include <unordered_map>

#include "structs.h"
#include "itablemanager.h"

using TableIndex = std::set<TableRow>;

class TableManager : public ITableManager
{
public:
	TableManager();

	CompleteOperationStatus Insert(const std::string& aTableName, const TableRow& aRow);
	CompleteOperationStatus Truncate(const std::string& aTableName);

	CompleteOperationStatus Intersection();
	CompleteOperationStatus SymmetricDifference();

	std::string Dump();
	
private:
	std::unordered_map<std::string, TableIndex> mTables;
};