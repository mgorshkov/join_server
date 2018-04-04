#pragma once

CommandExecutor::CommandExecutor(ITableManager* aTableManager)
	: mTableManager(aTableManager)
{
}

CompleteStatus CommandExecutor::RunCommand(const CompleteCommand& aCommand)
{
	return mTableManager.
}

CompleteCommand CommandExecutor::Parse(const std::string& aLine)
{
	if (aLine.find(CommandInsert) == 0)
	{

	}
	else if (aLine.find(CommandTruncate) == 0)
	{

	}
	else if (aLine.find(CommandIntersect) == 0)
	{

	}
	else if (aLine.find(CommandSymmetricDifference) == 0)
}

