#pragma once

class ITableManager
{
public:
	virtrual ~ITableManager() = default;

	virtual TableManagerStatus Insert(const std::string& aTableName, const TableRow& aRow) = 0;
	virtual TableManagerStatus Truncate(const std::string& aTableName) = 0;

	virtual TableManagerStatus Intersection() = 0;
	virtual TableManagerStatus SymmetricDifference() = 0;
};