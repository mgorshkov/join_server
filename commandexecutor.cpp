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
	auto it = aLine.find(CommandInsert);
	if (it != aLine.end())
		return HandleInsert(aLine.substr(it, aLine.end()));
	it = aLine.find(CommandTruncate);
	if (it != aLine.end())
		return HandleTruncate(aLine.substr(it, aLine.end()));
	it = aLine.find(CommandIntersect);
	if (it != aLine.end())
		return HandleIntersect(aLine.substr(it, aLine.end()));
	it = aLine.find(CommandSymmetricDifference);
	if (it != aLine.end())
		return HandleSymmetricDifference(aLine.substr(it, aLine.end()));
	
	return InvalidCommand;
}

CompleteCommand CommandExecutor::HandleInsert(const std::string& aLine)
{
	auto it = aLine.find(' ');
	if (it == aLine.end())
		return InvalidCommand;

	int id = std::atoi(aLine.substr(0, it));
	return CompleteCommand{Command::Insert};
}

CompleteCommand CommandExecutor::HandleTruncate(const std::string& aLine)
{
	return CompleteCommand{Command::Truncate};
}

CompleteCommand CommandExecutor::HandleIntersect(const std::string& aLine)
{
	return CompleteCommand{Command::Intersect};
}

CompleteCommand CommandExecutor::HandleSymmetricDifference(const std::string& aLine)
{
	return CompleteCommand{Command::SymmetricDifference};
}

