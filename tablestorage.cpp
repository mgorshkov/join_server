#include <assert.h>
#include <sstream>

#include "tablestorage.h"

TableManager::TableManager()
{
}

CompleteOperationStatus TableManager::Create(const std::string& aTableName)
{
	auto tablesIt = mTables.find(aTableName);
	if (tablesIt != mTables.end())
		return CompleteOperationStatus{OperationStatus::TableAlreadyExists};
	mTables.emplace(std::make_pair(aTableName, TableIndex{}));
	return CompleteOperationStatus{};
}

CompleteOperationStatus TableManager::Insert(const std::string& aTableName, const TableRow& aRow)
{
	auto tablesIt = mTables.find(aTableName);
	if (tablesIt == mTables.end())
	{
		auto createRes = Create(aTableName);
		if (createRes.mStatus != OperationStatus::Ok)
			return createRes;
	}
	auto& tableIndex = tablesIt->second;
	auto result = tableIndex.emplace(1,"aRow");
//	if (!result.second)
//		return CompleteOperationStatus{OperationStatus::DuplicateRecord};
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
	std::stringstream str;

	auto tableAIt = mTables.find("A");
	if (tableAIt != mTables.end())
	{
		str << "A:" << std::endl;
		for (const auto& p : tableAIt->second)
			str << p.mId << "," << p.mName << std::endl;
	}

	auto tableBIt = mTables.find("B");
	if (tableBIt != mTables.end())
	{
		str << "B:" << std::endl;
		for (const auto& p : tableBIt->second)
			str << p.mId << "," << p.mName << std::endl;
	}

	return str.str();
}