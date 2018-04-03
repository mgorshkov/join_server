#include "tablestorage.h"

TableManager::TableManager()
{
	mTables["A"] = TableIndex();
	mTables["B"] = TableIndex();
}

TableManagerStatus TableManager::Insert(const std::string& aTableName, const TableRow& aRow)
{
	auto tablesIt = mTables.find(aTableName);
	if (tablesIt == mTables.end())
		return TableManagerStatus{OperationStatus::NoTable};
	auto result = tablesIt->emplace(std::make_pair(aRow.mId, aRow.mName));
	if (!result.second)
		return TableManagerStatus{OperationStatus::DuplicateRecord};
	return TableManagerStatus{};
}

TableManagerStatus TableManager::Truncate(const std::string& aTableName)
{
	auto tablesIt = mTables.find(aTableName);
	if (tablesIt == mTables.end())
		return TableManagerStatus{OperationStatus::NoTable};
	tablesIt->clear();
	return TableManagerStatus{};
}

TableManagerStatus TableManager::Intersection()
{
	auto tableAIt = mTables.find("A");
	assert (tableAIt != mTables.end());

	auto tableBIt = mTables.find("B");
	assert (tableBIt != mTables.end());

	std::stringstream str;

	if (tableAIt->size() < tableBIt->size())
	{
		for (const auto& p : *tableAIt)
		{
			auto bIt = tableBIt->find(p.first);
			if (bIt != tableBIt->end())
				str << p.first << "," << p.second << "," << bIt->second << std::endl;
		}
	}
	else
	{
		for (const auto& p : *tableBIt)
		{
			auto aIt = tableAIt->find(p.first);
			if (aIt != tableAIt->end())
				str << p.first << "," << aIt->second << "," << p.second << std::endl;
		}
	}
	return TableManagerStatus{OperationStatus::Ok, str.str()};
}

TableManagerStatus TableManager::SymmetricDifference()
{
	auto tableAIt = mTables.find("A");
	assert (tableAIt != mTables.end());

	auto tableBIt = mTables.find("B");
	assert (tableBIt != mTables.end());
	
	for (const auto& p : *tableAIt)
	{
		auto bIt = tableBIt->find(p.first);
		if (bIt == tableBIt->end())
			str << p.first << "," << p.second << "," << std::endl;
	}
	for (const auto& p : *tableBIt)
	{
		auto aIt = tableAIt->find(p.first);
		if (aIt == tableAIt->end())
			str << p.first << ",," << p.second << std::endl;
	}
}
