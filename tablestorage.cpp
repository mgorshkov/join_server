#include <assert.h>
#include <sstream>

#include "tablestorage.h"

TableManager::TableManager()
{
	mTables["A"] = TableIndex();
	mTables["B"] = TableIndex();
}

CompleteOperationStatus TableManager::Insert(const std::string& aTableName, const TableRow& aRow)
{
	auto tablesIt = mTables.find(aTableName);
	if (tablesIt == mTables.end())
		return CompleteOperationStatus{OperationStatus::NoTable};
	auto result = tablesIt->second.emplace(aRow);
	if (!result.second)
		return CompleteOperationStatus{OperationStatus::DuplicateRecord};
	return CompleteOperationStatus{};
}

CompleteOperationStatus TableManager::Truncate(const std::string& aTableName)
{
	auto tablesIt = mTables.find(aTableName);
	if (tablesIt == mTables.end())
		return CompleteOperationStatus{OperationStatus::NoTable};
	tablesIt->second.clear();
	return CompleteOperationStatus{};
}

CompleteOperationStatus TableManager::Intersection()
{
	auto tableAIt = mTables.find("A");
	assert (tableAIt != mTables.end());

	auto tableBIt = mTables.find("B");
	assert (tableBIt != mTables.end());

	std::stringstream str;

	if (tableAIt->second.size() < tableBIt->second.size())
	{
		for (const auto& p : tableAIt->second)
		{
			auto bIt = tableBIt->second.find(TableRow{p.mId});
			if (bIt != tableBIt->second.end())
				str << p.mId << "," << p.mName << "," << bIt->mName << std::endl;
		}
	}
	else
	{
		for (const auto& p : tableBIt->second)
		{
			auto aIt = tableAIt->second.find(TableRow{p.mId});
			if (aIt != tableAIt->second.end())
				str << p.mId << "," << aIt->mName << "," << p.mName << std::endl;
		}
	}
	return CompleteOperationStatus{OperationStatus::Ok, str.str()};
}

CompleteOperationStatus TableManager::SymmetricDifference()
{
	auto tableAIt = mTables.find("A");
	assert (tableAIt != mTables.end());

	auto tableBIt = mTables.find("B");
	assert (tableBIt != mTables.end());

	std::stringstream str;
	
	for (const auto& p : tableAIt->second)
	{
		auto bIt = tableBIt->second.find(TableRow{p.mId});
		if (bIt == tableBIt->second.end())
			str << p.mId << "," << p.mName << "," << std::endl;
	}
	for (const auto& p : tableBIt->second)
	{
		auto aIt = tableAIt->second.find(TableRow{p.mId});
		if (aIt == tableAIt->second.end())
			str << p.mId << ",," << p.mName << std::endl;
	}
	return CompleteOperationStatus{OperationStatus::Ok, str.str()};
}

std::string TableManager::Dump()
{
	return std::string{};
}