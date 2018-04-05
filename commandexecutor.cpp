#include "commandexecutor.h"

CommandExecutor::CommandExecutor(ITableManager* aTableManager)
	: mTableManager(aTableManager)
{
}

CompleteOperationStatus CommandExecutor::RunCommand(const std::string& aLine)
{
    CompleteCommand command = Parse(aLine);
    switch (command.mCommand)
    {
    case Command::Insert:
	return mTableManager->Insert(command.mTableName, command.mRow);
    case Command::Truncate:
	return mTableManager->Truncate(command.mTableName);
    case Command::Intersection:
	return mTableManager->Intersection();
    case Command::SymmetricDifference:
	return mTableManager->SymmetricDifference();
    }
}

CompleteCommand CommandExecutor::Parse(const std::string& aLine)
{
	auto pos = aLine.find(CommandInsert);
	if (pos != std::string::npos)
		return HandleInsert(aLine.substr(pos, aLine.size()));
	pos = aLine.find(CommandTruncate);
	if (pos != std::string::npos)
		return HandleTruncate(aLine.substr(pos, aLine.size()));
	pos = aLine.find(CommandIntersection);
	if (pos != std::string::npos)
		return HandleIntersection(aLine.substr(pos, aLine.size()));
	pos = aLine.find(CommandSymmetricDifference);
	if (pos != std::string::npos)
		return HandleSymmetricDifference(aLine.substr(pos, aLine.size()));
	
	return CompleteCommand{Command::Error};
}

CompleteCommand CommandExecutor::HandleInsert(const std::string& aLine)
{
	auto pos = aLine.find(' ');
	if (pos == std::string::npos)
		return CompleteCommand{Command::Error};

	int id = std::atoi(aLine.substr(0, pos).c_str());
	return CompleteCommand{Command::Insert};
}

CompleteCommand CommandExecutor::HandleTruncate(const std::string& aLine)
{
	return CompleteCommand{Command::Truncate};
}

CompleteCommand CommandExecutor::HandleIntersection(const std::string& aLine)
{
	return CompleteCommand{Command::Intersection};
}

CompleteCommand CommandExecutor::HandleSymmetricDifference(const std::string& aLine)
{
	return CompleteCommand{Command::SymmetricDifference};
}

