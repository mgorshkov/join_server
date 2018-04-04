#pragma once

class CommandExecutor
{
public:
	CommandExecutor(ITableManager* aTableManager);

	CompleteStatus RunCommand(const std::string& aLine);

private:
	CompleteCommand Parse(const std::string& aLine);

	ITableManager* mTableManager;

	static const char* CommandInsert = "INSERT";
	static const char* CommandTruncate = "TRUNCATE";
	static const char* CommandIntersection = "INTERSECTION";
	static const char* CommandSymmetricDifference = "SYMMETRIC_DIFFERENCE";
};