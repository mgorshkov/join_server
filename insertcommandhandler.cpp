#include "insertcommandhandler.h"

InsertCommandHandler::InsertCommandHandler(ITableManager* aTableManager)
	: CommandHandler(aTableManager)
{		
}

std::string InsertCommandHandler::GetCommand() const
{
	return "INSERT";
}

CompleteCommand InsertCommandHandler::Parse(const std::string& aLine)
{
	auto pos = aLine.find(' ');
	if (pos == std::string::npos)
		return CompleteCommand{Command::Error};

	int id = std::atoi(aLine.substr(0, pos).c_str());
	return CompleteCommand{Command::Insert};
}

CompleteOperationStatus InsertCommandHandler::Handle(const CompleteCommand& aCommand)
{
	return mTableManager->Insert(aCommand.mTableName, aCommand.mRow);    
}

